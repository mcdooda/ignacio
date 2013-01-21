#include "syscall.h"
#define THIS "aaa"
#define THAT "bbb"

const int N = 1; // Choose it large enough!

void puts_(char *s) {
	char *p;
	for (p = s; *p != '\0'; p++) PutChar(*p);
}

void f(void *s) {
	int i;
	for (i = 0; i < N; i++) puts_((char *) s);
}

int main() {
/*
	int i;
	i = UserThreadCreate(f, (void *) THAT);
*/
	f((void*) THIS);
	return 0;
}
