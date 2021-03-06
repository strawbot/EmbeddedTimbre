// Machines for tasks  Rob Chapman  Dec 30, 2009

// use compiler flags to select atomic protection or not
// and for tracking under/overflows

// add depth detector for runMachines. Each time it is entered, depth increases by 1
// each time it is finished, depth decreases by 1
// add protection to runMachines so it can tell if it is recursive
extern "C" {

#include "machines.h"
#include "cli.h"
#include "printers.h"
#include <string.h>

void init_te();

QUEUE(MACHINES, machineq); // workers
QUEUE(ACTIONS, actionq); // now

Byte mmoverflow = 0, mmunderflow = 0;
Byte amoverflow = 0;

void next(vector machine) { // actions
	ATOMIC_SECTION_ENTER;
	if (leftq(actionq))
		pushq((Cell)machine, actionq);
	else {
		amoverflow++;
		while (true);
	}
	ATOMIC_SECTION_LEAVE;
}

void later(vector machine) {
	ATOMIC_SECTION_ENTER;
	if (leftq(machineq))
		pushq((Cell)machine, machineq);
	else {
		mmoverflow++;
		while (true);
	}
	ATOMIC_SECTION_LEAVE;
}

void stopAction(vector action) {
	ATOMIC_SECTION_ENTER;
	deq((Cell)action, actionq);
	ATOMIC_SECTION_LEAVE;
}

void deactivate(vector machine) { // remove a machine from queue
	ATOMIC_SECTION_ENTER;
	deq((Cell)machine, machineq);
	ATOMIC_SECTION_LEAVE;
}

void activateMachineOnce(vector machine) // only have one occurrance of a machine running
{
	deactivate(machine);
	later(machine);
}

static Long runDepth = 0;

void runMachines(void) {  // run all machines
	Byte n = (Byte)(queryq(machineq) + queryq(actionq));

	runDepth++;
	do {
		vector machine;

		if (queryq(actionq)) {// TODO: actions can stall out machines: limited by n
			safe(machine = (vector)pullq(actionq));
		} else if (queryq(machineq)) {
			safe(machine = (vector)pullq(machineq));
		} else {
			mmunderflow++;
			break;
		}
		machineRun(machine);

	} while (n--);
	runDepth--;
}

bool runmac(Qtype * mq) {
	if (queryq(mq)) {
		machineRun((vector)pullq(mq));
		return true;
	}
	return false;
}

// Multiple Machine queues
typedef struct MQueue {
	struct MQueue * link;
	Qtype * mq;
} MQueue;

// runtimeqs
// runs machines from all machineq's with priority given to top mq's after running a machine
// when an mq is empty, then the next one will be run
// this is different than the runMachines() which will run only the snapshot count of machines
// queued up to run at the start before exiting.
// Contrarily, runMachineqs() will not exit until all machines have been run. This is good
// for running low power modes after all the work has been done.
// runMachines() is good for getting out of a sticky spot where code is stuck waiting

MQueue workerq  = {.link = NULL,         .mq = machineq};
MQueue timeactionq = {.link = &workerq,  .mq = actionq };

MQueue * mqq = &timeactionq;

void runMachineqs() {
	MQueue * mqi = mqq;
	do {
		if (runmac(mqi->mq)) {
			mqi = mqq;
            continue;
		}
		mqi = mqi->link;
	} while (mqi != 0);
}

QUEUE(100, dids);
bool monitorActions = false;

void actions() {
	while (queryq(actionq)) {
		Cell action = pullq(actionq);
		if (monitorActions)  if (leftq(dids))  pushq(action, dids);
		machineRun((vector)action);
	}
}

char * getMachineName(Cell x);

void didWhat() {
	while (queryq(dids)) {
		print(" "), print(getMachineName(pullq(dids)));
	}
}

bool machines() {
	return runmac(machineq);
}

Long allThings() {
	return queryq(actionq) + queryq(machineq);
}

// endless loop for endless machines; returns for machines that wait
void run_till_done() {
	do actions();
	while (machines());
}

// run at most the number of machines at the moment - a slice or up to a whole of a cycle
void run_slice() {
	for (int i = allThings(); i != 0; i--) {
		actions();
		if (!machines())
		   break;
	}
}

//notes:
// slice() will run actions with priority;
// run_till_done() will not run actions as the first thing; possibly coming back from hibernate - critical because actions could get stagnant
// fixed by reworking control structures

// viewers
#include "dictionary.h"
#include "printers.h"

#define UNKNOWN "unknown_machines"

HASHDICT(HASH9, macnames); // keep track of machine names
HASHDICT(HASH9, mactimes); // keep track of machine max execution times

void listMachineNames() {
	int i = 0;
	dictionary_t *dict = &macnames;
	for (char * entry = dictFirst(dict); entry; entry = dictNext(dict), i++)
		print("\n"), print((char *)dict->adjunct[dict->iter]), printHex((Cell)entry);
	print("\n#names:"), printDec(i);
}

void initMachineStats() {
    emptyDict(&macnames);
    emptyDict(&mactimes);
    machineName((vector)(Cell)UNKNOWN, UNKNOWN);
}

void zeroMachineTimes() {
    for (Short i=0; i<mactimes.capacity; i++)
        mactimes.adjunct[i] = 0;
}

void machineName(vector machine, const char * name) // give name to machine
{
	dictAddKey((Cell)machine, &macnames);
	dictAddKey((Cell)machine, &mactimes);
	*dictAdjunctKey((Cell)machine, &macnames) = (Cell)name;
}

void activateOnceNamed(vector machine, const char * name) {
	activateMachineOnce(machine);
	machineName(machine, name);
}

char * getMachineName(Cell x) {
	char * name = *(char **)dictAdjunctKey(x, &macnames);

	if (name)
		return name;
    return (char *)"";
}

void showMachineName(Cell x)
{
	print("\n "), printHex(x), print(": ");
    print(getMachineName(x));
}

vector getMachine(char * name) { // return address of named machine
	for (Short i=0; i<macnames.capacity; i++)
		if (macnames.adjunct[i] != 0)
            if (strcmp((char *)macnames.adjunct[i], name) == 0)
			    return (vector)(Cell)macnames.table[i];
	return NULL;
}

void listq(Qtype *q) { // list q items
	Byte n;

	n = (Byte)queryq(q);
	{
		Cell l[n];

		ATOMIC_SECTION_ENTER;
		for (Byte i=0; i<n; i++) {
			l[i] = pullq(q);
			pushq(l[i], q);
		}
		ATOMIC_SECTION_LEAVE;
		for (Byte i=0; i<n; i++)
			showMachineName(l[i]);
	}
}

void listTimeEvents();

void listMachines(void) {
	print("\nmachineq:\n Depth: "), printDec(runDepth);
	listq(machineq);
	if (mmunderflow)
		print("\n munderflows: "), printDec(mmunderflow);
	if (mmoverflow)
		print("\n moverflows: "), printDec(mmoverflow);
	if (queryq(actionq))  print("\nactionq:"), listq(actionq);
	listTimeEvents();
}

void listm(void) { // list machine statuses
	later(listMachines);
}

static void runAction();

void initMachines(void) {
	zeroq(machineq);
	zeroq(actionq);
	initMachineStats();
	zeroq(dids);
	init_te();
	nameMachine(runAction);
}

void killMachine() {
	vector mac = getMachine((char *)parseWord(0));

	if (mac)
		deactivate(mac);
	else
		print("Machine not available.\n");
}

void activateMachine() {
	vector mac = getMachine((char *)parseWord(0));

	if (mac)
		later(mac);
	else
		print("Machine not available.\n");
}

// Machine cycle timer
#include "codeStats.c"
#include "printers.h"
#include <stdlib.h>

void machineRun(vector m) {
	Cell * stat = dictAdjunctKey((Cell)m, &mactimes);
	if (stat == 0) {
		machineName(m, NULL);
		stat = dictAdjunctKey((Cell)m, &mactimes);
	}

	int time = (int)getTicks();
	m();
	time = (int)getTicks() - time;

	if (time > (int)*stat) // TODO: consider atomicity of readnwrite
		*stat = (Cell)time;
}

static int indexCompare(const void *a,const void *b) {
	Short *x = (Short *) a;
	Short *y = (Short *) b;
	return mactimes.adjunct[*y] - mactimes.adjunct[*x];
}

void machineStats(void) {
	Short indexes[mactimes.capacity];
	Short j=0;

	for (Short i=0; i<mactimes.capacity; i++)
		if (mactimes.adjunct[i] != 0)
			indexes[j++] = i;

	qsort(indexes, j, sizeof(Short), indexCompare);

	for (Short i=0; i<j; i++) {
		Cell machine = (Cell)mactimes.table[indexes[i]];
		char * name = (char *)macnames.adjunct[indexes[i]];
		printCr();
		Cell time = mactimes.adjunct[indexes[i]];
		Long us = CONVERT_TO_US(time);

		if (us > 9999)
			dotnb(7, 6, CONVERT_TO_MS(time), 10), print(" ms  ");
		else if (us)
			dotnb(7, 6, us, 10), print(" us  ");
		else
			dotnb(7, 6, CONVERT_TO_NS(time), 10), print(" ns  ");

		if (name)
			print(name);
		else
			printHex(machine);
	}
}

}

// extensions for C++ in the same manner as events
static QUEUE(2*ACTIONS, unaq);

static void runAction() {
	unafun unary = (unafun)pullq(unaq);
	void * object = (void *)pullq(unaq);

	callUnary(object, unary);
}

void callUnary(void * object, unafun unary) {
	if (unary == NULL || object == NULL) 
	 	while (true);

	Cell * stat = dictAdjunctKey((Cell)unary, &mactimes);
	if (stat == 0) {
		machineName((vector)unary, NULL);
		stat = dictAdjunctKey((Cell)unary, &mactimes);
	}

	int time = (int)getTicks();
	unary(object);
	time = (int)getTicks() - time;

	if (time > (int)*stat) // TODO: consider atomicity of readnwrite
		*stat = (Cell)time;
}

Long nextOverflows = 0, laterOverflows = 0;

void next(void* object, unafun unary, const char * name) {
	if (leftq(unaq) < 2) {
		nextOverflows++;
		while (true);
	}
	ATOMIC_SECTION_ENTER;
	pushq((Cell)unary, unaq), pushq((Cell)object, unaq);
	ATOMIC_SECTION_LEAVE;
	next(runAction);
	(void)name;
}

void later(void* object, unafun unary, const char * name) {
	if (leftq(unaq) < 2) {
		laterOverflows++;
		while (true);
	}
	ATOMIC_SECTION_ENTER;
	pushq((Cell)unary, unaq), pushq((Cell)object, unaq);
	ATOMIC_SECTION_LEAVE;
	later(runAction);
	(void)name;
}

void stopAction(void* object, unafun unary) {
	ATOMIC_SECTION_ENTER;
	for (Long n = queryq(unaq)/2; n; n--) {
		Cell u = pullq(unaq), o = pullq(unaq);
		
		if (o == (Cell)object && u == (Cell)unary)
			stopAction(runAction);
		else
			pushq(u, unaq), pushq(o, unaq);
	}
	ATOMIC_SECTION_LEAVE;
}


