#include "syscall.h"

int main(int argc, char* argv[]) {
	OpenFileId fd1, fd2;
/*
	char buf1[] = {'h', 'e', 'l', 'l', 'o'};
	char buf2[sizeof(buf1)];
	int i;
*/
	
	Create("file");
/*
	Write(buf1, sizeof(buf1), fd1);
	Close(fd1);
*/
	
	fd1 = Open("file");
	fd2 = Open("file");
/*
	Read(buf2, sizeof(buf2), fd2);
	Close(fd2);
	
	for (i = 0; i < sizeof(buf2); i++) {
		PutChar(i);
	}
	PutChar('\n');
*/
	
	return 0;
}

