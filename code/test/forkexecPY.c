#include "syscall.h"

int main() {
	ForkExec("build/fork_test1");

	ForkExec("build/userpages1");
	ForkExec("build/userpages1");

/*
	ForkExec("userpages1");
*/
/*
	PutString("TEST\n");
*/
	PutChar('x');
	PutChar('y');
	PutChar('z');
	return 20;
}
