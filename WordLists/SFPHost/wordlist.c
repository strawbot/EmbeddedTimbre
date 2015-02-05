// System dictionaries  generated by parsewords.py  Feb 05, 2015  10:30:12

// names are kept in flash; arrays are used; reduces space requirements
#include "botkernl.h"

#define NAMES(name) PROGMEM char name[] = {
#define NAME(s) s "\000"
#define END_NAMES };

#define BODIES(functions) const void * const functions[] = {
#define BODY(f) (void * const)f,
#define CONSTANTBODY(f)  (void * const)CII,(void * const)&f,
#define END_BODIES };

void CII(void);

// Names and function binding for words, constants and immediates
// Names and function binding for words, constants and immediates

// Words
NAMES(wordnames)
// Kernel
	NAME("swap")		//  ( n m - m n ) swap top two items on the data stack
	NAME("dup")		//  ( n - n n ) make a copy of the top data stack item
	NAME("drop")		//  ( n - ) throw away the top data stack item
	NAME(">r")		//  ( n - ) (R - n ) push the top item of the data stack onto the return stack
	NAME("r")		//  ( - n ) (R n - n ) copy the top item of the return stack onto the data stack
	NAME("r>")		//   ( - n ) (R n - ) move top item on return stack to data stack
	NAME("sp!")		//  ( ... - ) empty the data stack
	NAME("over")		//  ( n m - n m n ) copy 2nd data stack item to top of data stack
	NAME("?dup")		//  ( n - n n | - 0 ) duplicate top data stack item if not 0
	NAME("execute")		//  ( a - ) use the top data stack item as a function call
	NAME("and")		//  ( n m - p ) bitwise AND top two data stack items and leave on top
	NAME("or")		//   ( n m - p ) bitwise OR top two data stack items and leave on top
	NAME("xor")		//  ( n m - p ) bitwise XOR top two data stack items and leave on top
	NAME("not")		//  ( n - n' ) invert all bits on the top data stack item
	NAME("+")		//  ( n m - p ) add top two data stack items and leave on top
	NAME("-")		//  ( n m - p ) subtract top data stack item from next item and leave on top
	NAME("negate")		//  ( n - -n ) two's complement of top data stack item
	NAME("/mod")		//  ( n m - q r ) return divide and modulus from top item into next item
	NAME("/")		//  ( n m - q ) divide next data stack item by top and leave on top
	NAME("mod")		//  ( n m - r ) modulus next data stack item by top and leave on top
	NAME("*")		//  ( n m - p ) multiply next data stack item by top and leave on top
	NAME("=")		//  ( n m - f ) leave a boolean on stack after equating top two data stack items
	NAME("<")		//  ( n m - f ) leave a boolean on stack indicating if next is less than top
	NAME(">")		//  ( n m - f ) leave a boolean on stack indicating if next is greater than top
	NAME("u<")		//  ( u v - f ) leave a boolean on stack indicating if unsigned next is less than top
	NAME("u>")		//  ( u b - f ) leave a boolean on stack indicating if unsigned next is greater than top
	NAME("abs")		//  ( n - n|-n) top data stack item is made positive
	NAME("max")		//  ( n m - n|m) leave maximum of top two stack items
	NAME("min")		//   ( n m - n|m) leave minimum of top two stack items
	NAME("@")		//  ( a - n ) return contents of memory using top stack item as the address (processor sized)
	NAME("!")		//  ( n a - ) store next into memory using top as address (processor sized)
	NAME("c@")		//  ( a - c ) return contents of memory using top stack item as the address (8 bit)
	NAME("c!")		//  ( c a - ) store next into memory using top as address (8 bit)
	NAME("s@")		//  ( a - h ) return contents of memory using top stack item as the address (16 bit)
	NAME("s!")		//  ( h a - ) store next into memory using top as address (16 bit)
	NAME("+b")		//  ( b a - ) turn on b bits at address a: 0b10001 em +b
	NAME("-b")		//  ( b a - ) turn off b bits at address a: 0b10001 em -b
	NAME("l@")		//  ( a - n )return contents of memory using top stack item as the address (32 bit)
	NAME("l!")		//  (n a - )store next into memory using top as address (processor sized)
	NAME("cmove")		//  ( s d n - ) move n bytes from s to d
	NAME("fill")		//  ( s n x - )fill n bytes from s with x
	NAME("erase")		//  ( s n - ) erase n bytes from s
	NAME("here")		//  ( - a ) return address of end of dictionary
	NAME("allot")		//  ( n - ) reserve n bytes after end of dictionary
	NAME("c,")		//  ( c - ) allocate and 1 byte and put value in it
	NAME(",")		//  ( n - ) allocate 1 cell and put n into it
	NAME("koff")		//  turn off automatic key echo
	NAME("kon")		//  turn on automatic key echo
	NAME("emit")		//  ( c - ) send c to output device
	NAME("cr")		//  send end of line to output device
	NAME("count")		//  ( a - a' c ) leave first character and incremented address on stack
	NAME("type")		//  ( a n - ) output n characters starting at a
	NAME("base")		//  ( - a ) return address of number radix
	NAME("hex")		//  interpret all following numbers as hex
	NAME("decimal")		//  interpret all subsequent numbers as decimal
	NAME("hold")		//  ( c - ) hold a character in number sequence
	NAME("<#")		//  inititiate a number sequence
	NAME("#")		//  ( n - n' ) convert a digit from n
	NAME("#s")		//  ( n - 0 ) convert all digits in n
	NAME("sign")		//  ( n - ) prepend sign to number sequence if n is negative
	NAME("#>")		//  ( n - a c ) finish number sequence and return address and count
	NAME(".r")		//  ( m n - ) print m in right field of n digits
	NAME(".")		//  ( n - ) print n in current number base
	NAME("]")		//  enter macro mode
	NAME("word")		//  ( c - ) parse characters up to c from input to here
	NAME(">b")		//  ( a - ) put machine into execution queue
	NAME(":")		//  start a macro definition
	NAME("constant")		//  ( n - ) give n a name
	NAME("variable")		//  ( n - ) give n a place to be stored at a name
	NAME("shift")		//  ( n m - p ) shift n by m bit left for minus and right for positive
	NAME(".b")		//  ( n - ) print n in binary
	NAME(".d")		//  ( n - ) print n in decimal
	NAME(".h")		//  ( n - ) print n in hex
	NAME(".s")		//  print number of items on data stack
	NAME("dump")		//  ( a n - ) dump n 16-byte rows of memory starting at address a
	NAME("words")		//  list all words in dictionary
	NAME("help")		//  print list of one liners about words in dictionary
	NAME("ls")		//  display link statistics
	NAME("cs")		//  set link statistics to zero
	NAME("las")		//  list all stats
	NAME("cas")		//  clear all stats
	NAME("dl")		//  display link state
	NAME("df")		//  dump link frame buffers
	NAME("load")		//  ( link n - ) load the sfp link with n frames
	NAME("ping")		//  ( i n - ) get response time to n pings to link i
	NAME("lm")		//  list machines
	NAME("n1")		//  switch to node n1
	NAME("n2")		//  switch to node n2
	NAME("n3")		//  switch to node n3
	NAME("n4")		//  switch to node n4
	NAME("n5")		//  switch to node n5
	NAME("n6")		//  switch to node n6
	NAME("n7")		//  switch to node n7
	NAME("n8")		//  switch to node n8
	NAME("start")		//  note start of an interval
	NAME("end")		//  end and report interval length
	NAME("time")		//  display timestamp
	NAME("t")		//  show ticks
	NAME("ton")		//  trace packet flow
	NAME("toff")		//  disable trace packet flow
	NAME("paths")		//  test all possible paths
	NAME("blitz")		//  start up a blitz of packets
	NAME("boff")		//  shutdown blitz
	NAME("glitch")		//  ( l ) generate a glitch on link l
	NAME("spson")		//  ( l ) enable SPS on link l
	NAME("spsoff")		//  ( l ) disable SPS on link l
	NAME("allsps")		//  turn on sps for all links
	NAME("nosps")		//  turn off sps for all links
	NAME("nodeup")		//  ( n ) enable a node for sfp traffic
	NAME("nodedown")		//  ( n ) disable a node for sfp traffic
	NAME("noise")		//  ( n ) add n amount of noise; 0 turns it off
	NAME("spsfails")		//  show all the sps failures
END_NAMES

// Kernel
void SWAP(void);
void DUP(void);
void DROP(void);
void TO_R(void);
void R(void);
void R_FROM(void);
void SP_STORE(void);
void OVER(void);
void QUESTION_DUP(void);
void EXECUTE(void);
void AND(void);
void OR(void);
void XOR(void);
void NOT(void);
void PLUS(void);
void MINUS(void);
void NEGATE(void);
void SLASH_MOD(void);
void SLASH(void);
void MOD(void);
void STAR(void);
void EQUALS(void);
void LESS_THAN(void);
void GREATER_THAN(void);
void U_LESS_THAN(void);
void U_GREATER_THAN(void);
void ABS(void);
void MAX(void);
void MIN(void);
void FETCH(void);
void STORE(void);
void C_FETCH(void);
void C_STORE(void);
void shortFetch(void);
void shortStore(void);
void PLUS_BITS(void);
void MINUS_BITS(void);
void longFetch(void);
void longStore(void);
void CMOVE(void);
void FILL(void);
void ERASE(void);
void HERE(void);
void ALLOT(void);
void C_COMMA(void);
void COMMA(void);
void autoEchoOff(void);
void autoEchoOn(void);
void EMIT(void);
void _CR(void);
void COUNT(void);
void TYPE(void);
void base(void);
void HEX(void);
void DECIMAL(void);
void HOLD(void);
void START_NUMBER_CONVERSION(void);
void CONVERT_DIGIT(void);
void CONVERT_NUMBER(void);
void SIGN(void);
void END_NUMBER_CONVERSION(void);
void DOT_R(void);
void DOT(void);
void RIGHT_SQUARE_BRACKET(void);
void WORD(void);
void TO_BARON(void);
void COLON(void);
void CONSTANT(void);
void VARIABLE(void);
void SHIFT(void);
void DOT_B(void);
void DOT_D(void);
void DOT_H(void);
void DOT_S(void);
void dump(void);
void WORDS(void);
void help(void);
void listStats(void);
void clearStats(void);
void listAllStats(void);
void clearAllStats(void);
void linkStatus(void);
void dumpFrames(void);
void sfpLoadTest(void);
void sfpPingTest(void);
void listm(void);
void n1(void);
void n2(void);
void n3(void);
void n4(void);
void n5(void);
void n6(void);
void n7(void);
void n8(void);
void startTime(void);
void endTime(void);
void showTime(void);
void showTicker(void);
void traceOn(void);
void traceOff(void);
void runAllPathsTest(void);
void blitzOn(void);
void blitzOff(void);
void glitch(void);
void enableSps(void);
void disableSps(void);
void allSps(void);
void noSps(void);
void setNodeUp(void);
void setNodeDown(void);
void addNoise(void);
void showSpsFails(void);

BODIES(wordbodies)
// Kernel
	BODY(SWAP)
	BODY(DUP)
	BODY(DROP)
	BODY(TO_R)
	BODY(R)
	BODY(R_FROM)
	BODY(SP_STORE)
	BODY(OVER)
	BODY(QUESTION_DUP)
	BODY(EXECUTE)
	BODY(AND)
	BODY(OR)
	BODY(XOR)
	BODY(NOT)
	BODY(PLUS)
	BODY(MINUS)
	BODY(NEGATE)
	BODY(SLASH_MOD)
	BODY(SLASH)
	BODY(MOD)
	BODY(STAR)
	BODY(EQUALS)
	BODY(LESS_THAN)
	BODY(GREATER_THAN)
	BODY(U_LESS_THAN)
	BODY(U_GREATER_THAN)
	BODY(ABS)
	BODY(MAX)
	BODY(MIN)
	BODY(FETCH)
	BODY(STORE)
	BODY(C_FETCH)
	BODY(C_STORE)
	BODY(shortFetch)
	BODY(shortStore)
	BODY(PLUS_BITS)
	BODY(MINUS_BITS)
	BODY(longFetch)
	BODY(longStore)
	BODY(CMOVE)
	BODY(FILL)
	BODY(ERASE)
	BODY(HERE)
	BODY(ALLOT)
	BODY(C_COMMA)
	BODY(COMMA)
	BODY(autoEchoOff)
	BODY(autoEchoOn)
	BODY(EMIT)
	BODY(_CR)
	BODY(COUNT)
	BODY(TYPE)
	BODY(base)
	BODY(HEX)
	BODY(DECIMAL)
	BODY(HOLD)
	BODY(START_NUMBER_CONVERSION)
	BODY(CONVERT_DIGIT)
	BODY(CONVERT_NUMBER)
	BODY(SIGN)
	BODY(END_NUMBER_CONVERSION)
	BODY(DOT_R)
	BODY(DOT)
	BODY(RIGHT_SQUARE_BRACKET)
	BODY(WORD)
	BODY(TO_BARON)
	BODY(COLON)
	BODY(CONSTANT)
	BODY(VARIABLE)
	BODY(SHIFT)
	BODY(DOT_B)
	BODY(DOT_D)
	BODY(DOT_H)
	BODY(DOT_S)
	BODY(dump)
	BODY(WORDS)
	BODY(help)
	BODY(listStats)
	BODY(clearStats)
	BODY(listAllStats)
	BODY(clearAllStats)
	BODY(linkStatus)
	BODY(dumpFrames)
	BODY(sfpLoadTest)
	BODY(sfpPingTest)
	BODY(listm)
	BODY(n1)
	BODY(n2)
	BODY(n3)
	BODY(n4)
	BODY(n5)
	BODY(n6)
	BODY(n7)
	BODY(n8)
	BODY(startTime)
	BODY(endTime)
	BODY(showTime)
	BODY(showTicker)
	BODY(traceOn)
	BODY(traceOff)
	BODY(runAllPathsTest)
	BODY(blitzOn)
	BODY(blitzOff)
	BODY(glitch)
	BODY(enableSps)
	BODY(disableSps)
	BODY(allSps)
	BODY(noSps)
	BODY(setNodeUp)
	BODY(setNodeDown)
	BODY(addNoise)
	BODY(showSpsFails)
END_BODIES

// Immediates
NAMES(immediatenames)
	NAME("[")		//  exit macro mode
	NAME("(")		//  start of comment till end of line or )
	NAME("literal")		//  ( n - ) enter a literal value into a macro
	NAME("if")		//  ( n - ) execute following code if top of stack is non-zero
	NAME("else")		//  otherwise part of an if statement
	NAME("endif")		//  end of else or if statement
	NAME("begin")		//  start of a loop construct
	NAME("again")		//  end of a continuous loop construct
	NAME("while")		//  ( n - ) conditional choice in a loop construct
	NAME("repeat")		//  go back to the begin part
	NAME("until")		//  ( n - ) go back to the begin statement if stack is zero
	NAME("for")		//  ( n - ) start of a loop which runs n times
	NAME("next")		//  end of a for loop
	NAME("exit")		//  exit macro
	NAME("'")		//  ( - a ) get execution address of following word
	NAME("\"")		//  ( - s ) enclose text up to next " in a count prefixed string
	NAME(".\"")		//  print text up until next "
	NAME("reveal")		//  allow macro to call itself
	NAME(";")		//  end a macro
END_NAMES

void LEFT_SQUARE_BRACKET(void);
void COMMENT(void);
void LITERAL(void);
void IF(void);
void ELSE(void);
void ENDIF(void);
void BEGIN(void);
void AGAIN(void);
void WHILE(void);
void REPEAT(void);
void UNTIL(void);
void FOR(void);
void NEXT(void);
void EXIT(void);
void TICK(void);
void QUOTE(void);
void DOT_QUOTE(void);
void RECURSIVE(void);
void SEMI_COLON(void);

BODIES(immediatebodies)
	BODY(LEFT_SQUARE_BRACKET)
	BODY(COMMENT)
	BODY(LITERAL)
	BODY(IF)
	BODY(ELSE)
	BODY(ENDIF)
	BODY(BEGIN)
	BODY(AGAIN)
	BODY(WHILE)
	BODY(REPEAT)
	BODY(UNTIL)
	BODY(FOR)
	BODY(NEXT)
	BODY(EXIT)
	BODY(TICK)
	BODY(QUOTE)
	BODY(DOT_QUOTE)
	BODY(RECURSIVE)
	BODY(SEMI_COLON)
END_BODIES

// Constants
NAMES(constantnames)
	NAME("dbg")		//  set debug level 0 - quiet, 1 state changes, 2 traffic: 2 dbg c!
END_NAMES

extern Byte debug;

BODIES(constantbodies)
	CONSTANTBODY(debug)
END_BODIES

