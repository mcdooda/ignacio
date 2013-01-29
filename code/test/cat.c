#include "syscall.h"
#include "libio.h"

int main(int argc, char* argv[]) {
	OpenFileId fd;
	int nbLu;
	int i;
	char buf[10];


	fd = Open("file");
	printf("\n***\n");
	nbLu = Read(buf, sizeof (buf), fd);
	while (nbLu > 0) {
		for (i = 0; i < nbLu; i++) {
			PutChar(buf[i]);
		}
		nbLu = Read(buf, sizeof (buf), fd);
	}
	printf("\n***\n");
	Close(fd);


	return 0;
}


