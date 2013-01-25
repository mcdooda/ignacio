#include "syscall.h"
#include "libio.h"

void print(char c, int n) {
    int i;
    for (i = 0; i < n; i++) {
        PutChar(c + i);
    }
    PutChar('\n');
}

int main() {
    int c;
    print('a', 4);
    
    c = GetChar();
    
    while (c != EOF) {
        PutChar(c);
        c = GetChar();
    }

    return 0;
}
