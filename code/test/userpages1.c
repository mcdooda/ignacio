#include "syscall.h"
#define THIS "AAA"
#define THAT "BBB"

const int N = 10; // Choose it large enough!

void puts(char *s) {
	char *p;
	for (p = s; *p != '\0'; p++) PutChar(*p);
}

void f(void *s) {
	int i;
	for (i = 0; i < N; i++) puts((char *) s);
}

int main() {
	int i;
	i = UserThreadCreate(f, (void *) THIS);
	f((void*) THAT);
	return 0;
}
