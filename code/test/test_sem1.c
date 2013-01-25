#include "syscall.h"
#include "libsynch.h"

struct s {
	sem_t* sem;
	char c;
};

void SimpleThread(void* which) {
	int i;
	P(*((struct s*)which)->sem);
    for (i = 0; i < 1000; i++) {
        PutChar(((struct s*)which)->c);
        PutInt(i);
        PutChar('\n');
    }
	V(*((struct s*)which)->sem);
    UserThreadExit();
}

int
main() {
	sem_t sem;
	struct s a, b, c, d;
    int tid1;
    int tid2;
    int tid3;
    int tid4;
	sem = sem_create("usersem test", 1);
	a.sem = &sem; a.c = 'a';
	b.sem = &sem; b.c = 'b';
	c.sem = &sem; c.c = 'c';
	d.sem = &sem; d.c = 'd';
    tid1 = UserThreadCreate(SimpleThread, (void*) &a);
    tid2 = UserThreadCreate(SimpleThread, (void*) &b);
    tid3 = UserThreadCreate(SimpleThread, (void*) &c);
    tid4 = UserThreadCreate(SimpleThread, (void*) &d);

    UserThreadJoin(tid1);
    UserThreadJoin(tid2);
    UserThreadJoin(tid3);
    UserThreadJoin(tid4);
	sem_destroy(sem);
    /* not reached */
    return 0;
}
