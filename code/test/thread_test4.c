#include "syscall.h"

void Thread(void* arg) {
	char* c = (char*) arg;
	PutChar(*c);
	PutChar(*c);
	PutChar(*c);
}

int main(int argc, char** argv) {
	char a = 'a', b = 'b', c = 'c';
	int tidA, tidB, tidC;

	tidA = UserThreadCreate(Thread, &a);
	tidB = UserThreadCreate(Thread, &b);
	tidC = UserThreadCreate(Thread, &c);

	

	UserThreadJoin(tidA);
	UserThreadJoin(tidB);
	UserThreadJoin(tidC);


	return 0;
}

