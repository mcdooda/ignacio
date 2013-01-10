#include "syscall.h"

void print(char c, int n) {
    int i;
    for (i = 0; i < n; i++) {
        PutChar(c + i);
    }
    PutChar('\n');
}

int main() {
    char c;
    print('a', 4);
    
    c = GetChar();
    
    //TODO : EOF = -1 mais je sais pas quoi inclure pour que ca existe
    while (c != -1) {
        PutChar(c);
        c = GetChar();
    }

    return 18;
}
