#include "syscall.h"

int main() {
	ForkExec("userpages0");
	ForkExec("userpages1");
	PutString("TEST\n");
	return 0;
}
