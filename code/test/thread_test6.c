#include "syscall.h"

void thread(void* data) {
	int* n = (int*)data;
	PutString("n = ");
	PutInt(*n);
	PutChar('\n');
	if (*n > 0) {
		(*n)--;
		if (UserThreadCreate(thread, data) == -1)
			PutChar('Z');
		if (UserThreadCreate(thread, data) == -1)
			PutChar('Z');
	}
}

int main(int argc, char* argv[]) {
	int n = 3;
	thread(&n);
	return 0;
}

