// Time Event Action interface

#include "clocks.h"
#include "tea.h"
#include "queue.h"
#include "printers.h"
#include <stdlib.h>

// time tracker; ms and S
static Long uptime = 0;
static Long last_dueDate, zero_ms; // points on the number wheel

static void one_second() {
	uptime++;
	zero_ms = raw_time();
	in(secs(1), one_second);
}

Long getTime() { 
	Long ms = to_msec(raw_time() - zero_ms);
	return uptime*1000 + ms;
}

uint32_t getUptime() {
	return uptime;
}

void printUptime() {
    printDec(getUptime());
}

// primitives
static TimeEvent tes[NUM_TE], te_done, te_todo; // time events and todo and done lists
static void append(TimeEvent * curr, TimeEvent * te) {
	te->next = curr->next;
	curr->next = te;
}

static TimeEvent * remove(TimeEvent * curr) {
	TimeEvent * te = curr->next;
	curr->next = te->next;
	te->next = NULL;
	return te;
}

// list access
static void te_return(TimeEvent* te) { append(&te_done, te); }

static TimeEvent* te_borrow() {
	if (te_done.next == NULL)
		BLACK_HOLE();

	return remove(&te_done);	
}

// make it happen
static void do_action(TimeEvent * te) {
	bool asap = te->asap;
	vector action = te->action;
	te_return(te);

	if (asap)
		now(action);
	else
		later(action);
}

static void run_dueDate() { do_action(remove(&te_todo)); }

static void set_next_dueDate() {
	while (te_todo.next && !set_dueDate(te_todo.next->dueDate))
		run_dueDate();
}

static void schedule_te(TimeEvent* te) {
	TimeEvent * curr = &te_todo, * next;
	Long ref = last_dueDate; // time reference
	int dueDate = te->dueDate - ref;

	while ((next = curr->next) != NULL) {
		int date = next->dueDate - ref;
		if (dueDate < date)
			break;
		curr = next;
	}
	append(curr, te);
}

static void in_after(Long t, vector action, bool asap) {
	TimeEvent * te;

	safe( te = te_borrow(); )

	te->action = action;
	te->dueDate = get_dueDate(t);
	te->asap = asap;

	safe(schedule_te(te);)
	set_next_dueDate();
}

// Time Events
void after(Long t, vector action) { in_after(t, action, false); }
void in   (Long t, vector action) { in_after(t, action, true);  }

void check_dueDates(Long dueDate) {
	last_dueDate = dueDate;
	set_next_dueDate();
}

// Events
void no_action() {}

void when(Event e, vector a) { *e = a; }
void never(Event e) { when(e, no_action); }

// Actions
static QUEUE(NUM_ACTIONS, actionq);

void later(vector a) {
	if (leftq(actionq) == 0)
		BLACK_HOLE();
	CORE_ATOMIC_SECTION( pushq((Cell)a, actionq);)
}

void run() {
	while (queryq(actionq))  actionRun((vector)pullq(actionq));
}

void action_slice() { // like run but only once through the queued actions; full slice
	Long n = queryq(actionq);
	while (n--) actionRun((vector)pullq(actionq));
}

// reductions
void stop_te(vector v) {
	CORE_ATOMIC_SECTION(
	TimeEvent* te = te_todo.next;
	while (te) {
		if (te->action == v)  te->action = no_action;
		te = te->next;
	})
}

void stop_action(vector v) {
	CORE_ATOMIC_SECTION(
	for (Long n = queryq(actionq); n; n--) {
		Cell a = pullq(actionq);
		if (a != (Cell)v)  pushq(a, actionq); // develop iter q tool to read and write without moving items
	})
}

void stop(vector v) {
	stop_te(v);
	stop_action(v);
}

// names for actions 
HASHDICT(HASH9, teanames); // keep track of machine names
HASHDICT(HASH9, teatimes); // keep track of machine max execution times

void actor(vector action, const char * name) { // give name to action
	dictAddKey((Cell)action, &teatimes);
	dictAddKey((Cell)action, &teanames);
	*dictAdjunctKey((Cell)action, &teanames) = (Cell)name;
}

static void printActionName(Cell x) {
	char ** name = (char **)dictAdjunctKey(x, &teanames);
	if (name)
		print(*name);
	else
		printHex(x-1);
}

static void printDueDate(Long dd) {
	dd = dd - get_dueDate(0);
	if (dd < secs(1))
		printDec(to_msec(dd)), print("msec  ");
	else if (dd < mins(5))
		printDec(to_secs(dd)), print("secs  ");
	else if (dd < hours(5))
		printDec(dd/mins(1)),  print("mins  ");
	else
		printDec(dd/hours(1)), print("hours ");
}

void print_te() {
	TimeEvent * curr = te_todo.next;
	show_timer();
	while (curr) {
		print (curr->asap ? "\nin " : "\nafter ");
		printDueDate(curr->dueDate);
		tabTo(17);
		printActionName((Cell)curr->action);
		curr = curr->next;
	}
}

void print_actions() {
	int n = queryq(actionq);
	print("\n#actions:"), printDec(n);
	while(n) {
		printCr();
		printActionName(q(actionq));
		rotateq(actionq, 1);
		n--;
	}
}

void dumpTeaNames() {
	for(int i = 0; i < HASH9; i++)
		if (teanamesadjunct[i])
			printCr(), printHex(teanamesadjunct[i]), print((char *)teanamesadjunct[i]);
}

static int indexCompare(const void *a,const void *b) {
	Short *x = (Short *) a;
	Short *y = (Short *) b;
	return teatimes.adjunct[*y] - teatimes.adjunct[*x];
}

#include "codeStats.c"
#define max(a,b) a > b ? a : b

static void print_elapsed_time(Cell time) {
	Long us = CONVERT_TO_US(time);
	if (us > 9999)
		dotnb(7, 6, CONVERT_TO_MS(time), 10), print(" ms  ");
	else if (us)
		dotnb(7, 6, us, 10), print(" us  ");
	else
		dotnb(7, 6, CONVERT_TO_NS(time), 10), print(" ns  ");
}

void machineStats(void) {
	Short indexes[teatimes.capacity];
	Short j=0;

	for (Short i=0; i<teatimes.capacity; i++)
		if (teatimes.adjunct[i] != 0)
			indexes[j++] = i;

	qsort(indexes, j, sizeof(Short), indexCompare);

	print(" mstats ");
	for (Short i=0; i<j; i++) {
		Cell machine = (Cell)teatimes.table[indexes[i]];
		char * name = (char *)teanames.adjunct[indexes[i]];
		printCr();
		print_elapsed_time(teatimes.adjunct[indexes[i]]);
		if (name)
			print(name);
		else
			printHex(machine - 1);
	}
}

Cell * action_stat(vector m) {
	Cell * stat = dictAdjunctKey((Cell)m, &teatimes);
	if (stat == 0) {
		actor(m, NULL);
		stat = dictAdjunctKey((Cell)m, &teatimes);
	}
	return stat;
}

void actionRun(vector m) {
	Cell * stat = action_stat(m);
	int time = (int)getTicks();
	m();
	time = (int)getTicks() - time;
	* stat = max(time, *stat);
}

void zeroMachineTimes() {
    for (Short i=0; i<teatimes.capacity; i++)
        teatimes.adjunct[i] = 0;
}

void initMachineStats() {
    emptyDict(&teanames);
    emptyDict(&teatimes);
	zeroMachineTimes();
}

// init
void init_tea() {
	initMachineStats();
	zeroq(actionq);
	te_todo.next = te_done.next = NULL;

	for (Byte i = 0; i < NUM_TE; i++)
		te_return(&tes[i]);

	zero_ms = raw_time();
	last_dueDate = get_dueDate(0);

	later(one_second);
	init_clocks();

	namedAction(set_next_dueDate);
	namedAction(one_second);
	namedAction(no_action);
}

// test vector
#include "timeout.h"
#include "cli.h"

void test_time() {
	Long s = ret();
	Long tick = getTicks();
	Long time = getTime();
	timeoutWait(secs(s));
	tick = getTicks() - tick;
	time = getTime() - time; 
	print("\n"), printDec0(s), print(" second in ticks to ms: "), printDec(CONVERT_TO_MS(tick));
	print("  in time(ms):"), printDec(time);
}

void get_tick_time() {
	Long tick = getTicks();
	Long time = getTime();
	printDec(tick), printDec(time);
}

void ticks_ms() { lit(CONVERT_TO_MS(ret())); }