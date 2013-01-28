#include "syscall.h"

int main(int argc, char* argv[]) {
	OpenFileId fd1, fd2;

	char buf1[6];
	char buf2[6];
	int i;
	buf1[0] = 'h';
	buf1[1] = 'e';
	buf1[2] = 'l';
	buf1[3] = 'l';
	buf1[4] = 'o';
	buf1[5] = '\n';


	Create("file");

	fd1 = Open("file");
	fd2 = Open("file");

	
	Write(buf1, sizeof(buf1), fd1);
	Close(fd1);

	Read(buf2, sizeof(buf2), fd2);
	Close(fd2);
	
	for (i = 0; i < sizeof(buf2); i++) {
		PutChar(buf2[i]);
	}
	PutChar('\n');

	
	return 0;
}

