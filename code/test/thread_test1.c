#include "syscall.h"

void SimpleThread(void* which) {
    char* ch = (char*) which;
    int i;
    for (i = 0; i < 10; i++) {
        PutChar(*ch);
        PutInt(i);
        PutChar('\n');
    }
    UserThreadExit();
}

int
main() {
    char c1 = 'a';
    char c2 = 'b';
    char c3 = 'c';
    char c4 = 'd';
    int j;
    int tid1;
    int tid2;
    int tid3;
    int tid4;

    tid1 = UserThreadCreate(SimpleThread, (void*) &c1);
    tid2 = UserThreadCreate(SimpleThread, (void*) &c2);
    tid3 = UserThreadCreate(SimpleThread, (void*) &c3);
    tid4 = UserThreadCreate(SimpleThread, (void*) &c4);

    for (j = 0; j < 10; j++) {
        PutChar('m');
        PutInt(j);
        PutChar('\n');
    }

    UserThreadJoin(tid1);
    UserThreadJoin(tid2);
    UserThreadJoin(tid3);
    UserThreadJoin(tid4);

    /* not reached */
    return 0;
}