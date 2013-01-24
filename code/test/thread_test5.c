#include "syscall.h"

void thread1(void* data) {
	int i;
	for (i = 0; i < 100; i++)
		PutChar('a');
	
	UserThreadJoin(*(int*)data);
}

void thread2(void* data) {
	int i;
	for (i = 0; i < 100; i++)
		PutChar('b');
}

int main(int argc, char* argv[]) {
	int tid2 = -1;
	UserThreadCreate(thread1, &tid2);
	tid2 = UserThreadCreate(thread2, (void*)0);
	return 0;
}

