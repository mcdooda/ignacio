#include "syscall.h"

int main() {

	ForkExec("fork1");


	ForkExec("usrpage1");

	ForkExec("usrpage1");


/*
	ForkExec("userpages1");
*/
/*
	PutString("TEST\n");
*/
	PutChar('x');
	PutChar('y');
	PutChar('z');
	return 0;
}
