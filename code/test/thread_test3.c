#include "syscall.h"

void Thread(void* arg) {
	char* c = (char*) arg;
	int i;
	for (i = 0; i < 50; i++) {
		PutChar(*c);
	}
	UserThreadExit();
}

void Thread2(void* arg) {
	int tid = *(int*) arg;
	int i;
	for (i = 0; i < 500; i++) {
		PutChar('d');
	}
	PutChar('Q');
	UserThreadJoin(tid);
	PutChar('Z');
	UserThreadExit();
}

int main(int argc, char** argv) {
	char a = 'a', b = 'b', c = 'c';
	int tidA, tidB, tidC;
	int tids[4];
	int i;

	tidA = UserThreadCreate(Thread, &a);
	tidB = UserThreadCreate(Thread, &b);
	tidC = UserThreadCreate(Thread, &c);
	
	for (i = 0; i < 4; i++) {
		tids[i] = UserThreadCreate(Thread2, &tidA);
	}

	UserThreadJoin(tidB);
	UserThreadJoin(tidC);
	
	for (i = 0; i < 4; i++) {
		UserThreadJoin(tids[i]);
	}

	return 0;
}

