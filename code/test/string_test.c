#include "syscall.h"

int main() {
    char buffer[10];


    GetString(buffer, 10);

    while (buffer[0] != -1) {
        PutChar('<');
        PutString(buffer);
        PutChar('>');
        GetString(buffer, 10);
    }

    return 18;
}