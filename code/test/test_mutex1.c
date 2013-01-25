#include "syscall.h"
#include "libsynch.h"

struct s {
	mutex_t* mut;
	char c;
};

void SimpleThread(void* which) {
	int i;
	mutex_P(*((struct s*)which)->mut);
    for (i = 0; i < 1000; i++) {
        PutChar(((struct s*)which)->c);
    }
	mutex_V(*((struct s*)which)->mut);
    UserThreadExit();
}

int
main() {
	mutex_t mut;
	struct s a, b, c, d;
    int tid1;
    int tid2;
    int tid3;
    int tid4;
	mut = mutex_create("usermut test");
	a.mut = &mut; a.c = 'a';
	b.mut = &mut; b.c = 'b';
	c.mut = &mut; c.c = 'c';
	d.mut = &mut; d.c = 'd';
    tid1 = UserThreadCreate(SimpleThread, (void*) &a);
    tid2 = UserThreadCreate(SimpleThread, (void*) &b);
    tid3 = UserThreadCreate(SimpleThread, (void*) &c);
    tid4 = UserThreadCreate(SimpleThread, (void*) &d);

    UserThreadJoin(tid1);
    UserThreadJoin(tid2);
    UserThreadJoin(tid3);
    UserThreadJoin(tid4);
	mutex_destroy(mut);

	return 0;
}
