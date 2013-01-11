#include "syscall.h"

int main() {
	//TODO controle-D buggé (du au terminal ?)
	char buffer[10];
	do {
		GetString(buffer, 10);
		PutChar('<');
		PutString(buffer);
		PutChar('>');
	} while (!(buffer[0] == 'e' && buffer[1] == 'x' && buffer[2] == 'i' && buffer[3] == 't' && buffer[4] == '\n') || buffer[0] != '\0');

	return 0;
}