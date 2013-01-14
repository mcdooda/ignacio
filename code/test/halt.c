/* halt.c
 *	Simple program to test whether running a user program works.
 *	
 *	Just do a "syscall" that shuts down the OS.
 *
 * 	NOTE: for some reason, user programs with global data structures 
 *	sometimes haven't worked in the Nachos environment.  So be careful
 *	out there!  One option is to allocate data structures as 
 * 	automatics within a procedure, but if you do this, you have to
 *	be careful to allocate a big enough stack to hold the automatics!
 */

#include "syscall.h"

void SimpleThread(void* which) {
	int* i = (int*) which;
	for (; *i > 0; (*i)--) {
/*
		PutChar('*');
*/
		PutInt(*i);
/*
		PutChar('\n');
*/
	}
	UserThreadExit();
}

int
main() {
	int i = 100;
	int j = 100;
	int tid;
	PutChar('a');
	PutInt(i);
	PutInt(j);
	tid = UserThreadCreate(SimpleThread, (void*) &i);
/*
	tid += 5;
	tid -= 5;
*/
	
	PutChar('T');
	PutInt(tid);
	PutChar('T');
	
	
	for (j = 100; j > 0; j--) {
		PutInt(j);
		PutChar('\n');
	}

	UserThreadJoin(tid);

	PutString("i = ");
	PutInt(i);
	PutString("j = ");
	PutInt(j);

	/* not reached */
	return 0;
}
