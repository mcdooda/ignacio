#include "syscall.h"
#include "libio.h"

int main(int argc, char* argv[]) {
	OpenFileId fd1;

	char buf1[900];

	int i,j;

	Create("file");

	fd1 = Open("file");

	for (i = 0; i < 100; i++) {
		for(j = 0; j < sizeof(buf1); j++) {
			buf1[j] = 'a'+(i%26);
		}
		buf1[sizeof(buf1)-1] = '*';
		Write(buf1, sizeof (buf1), fd1);
	}
	Close(fd1);



	return 0;
}

