#include "syscall.h"

int main() {
	int pid1, pid2, pid3;
	
	pid1 = ForkExec("fork_test1");


	pid2 = ForkExec("userpages1");

	pid3 = ForkExec("userpages1");


/*
	ForkExec("userpages1");
*/
/*
	PutString("TEST\n");
*/
	WaitPid(pid1);
	WaitPid(pid2);
	WaitPid(pid3);
	
	PutChar('x');
	PutChar('y');
	PutChar('z');
	PutChar('\n');
	return 0;
}
