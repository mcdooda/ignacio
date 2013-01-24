#include "syscall.h"

int main() {
	ForkExec("fork_test1");
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
