#include "syscall.h"

int main() {
	//TODO controle-D buggé (du au terminal ?)
    char buffer[10];
	int i;
    do {
		for(i = 0; i < 10; i++)
			buffer[i] = '\0';
		GetString(buffer, 10);
        PutChar('<');
        PutString(buffer);
        PutChar('>');
    } while (!(buffer[0] == 'e' && buffer[1] == 'x' && buffer[2] == 'i' && buffer[3] == 't') || buffer[0] != '\0');

    return 0;
}