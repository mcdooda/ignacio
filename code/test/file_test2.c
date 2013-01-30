#include "syscall.h"
#include "libio.h"
#include "libsynch.h"

OpenFileId fd;

void SimpleThread(void* which) {
	char buf[100];
	char* ch = (char*) which;
	int i;

	for (i = 0; i < sizeof (buf); i++) {
		buf[i] = (*ch);
	}
	Write(buf, sizeof (buf), fd);
	UserThreadExit();
}

int main(int argc, char* argv[]) {
	int tid1;
	int tid2;
	int tid3;
	int tid4;
	char c1 = 'a';
	char c2 = 'b';
	char c3 = 'c';
	char c4 = 'd';

	Create("file");
	
	fd = Open("file");

	tid1 = UserThreadCreate(SimpleThread, (void*) &c1);
	tid2 = UserThreadCreate(SimpleThread, (void*) &c2);
	tid3 = UserThreadCreate(SimpleThread, (void*) &c3);
	tid4 = UserThreadCreate(SimpleThread, (void*) &c4);

	UserThreadJoin(tid1);
	UserThreadJoin(tid2);
	UserThreadJoin(tid3);
	UserThreadJoin(tid4);
	
	Close(fd);

	return 0;
}


