#include "syscall.h"
#include "libio.h"

int main(int argc, char* argv[]) {
	OpenFileId fd1, fd2;

	char buf1[6];
	char buf2[6];
	char buf3[7];
	
	int i;
	int tot = 0;
	buf1[0] = 'h';
	buf1[1] = 'e';
	buf1[2] = 'l';
	buf1[3] = 'l';
	buf1[4] = 'o';
	buf1[5] = '\0';
	
	buf3[0] = 'f';
	buf3[1] = 'i';
	buf3[2] = 'l';
	buf3[3] = 'e';
	buf3[4] = 'a';
	buf3[5] = '0';
	buf3[6] = '\0';


	for(i = 0; i < 5; i++){
		tot += 10000;
		if(i / 26 > 0){
			buf3[5] = (i/26) + '0';
		}
		buf3[4] = (i % 26) + 'a';
		Create(buf3);
		printf("espace utilise : %d\n",&tot);
	}
	
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

