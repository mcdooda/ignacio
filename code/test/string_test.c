#include "syscall.h"

int main() {
    char buffer[10];

    do {
		GetString(buffer, 10);
        PutChar('<');
        PutString(buffer);
        PutChar('>');
    } while (buffer[0] != -1);

    return 0;
}