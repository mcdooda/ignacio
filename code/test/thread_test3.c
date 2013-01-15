/* 
 * File:   thread_test2.c
 * Author: gervaila
 *
 * Created on 15 janvier 2013, 11:38
 */

#include "syscall.h"

void Thread(void* arg) {
	int* i = (int*) arg;
	PutChar('T');
	PutInt(*i);
	PutChar('\n');
	(*i)--;
	if (*i > 0) {
		int tid = UserThreadCreate(Thread, arg);
		UserThreadJoin(tid);
	}
	UserThreadExit();
}

int main(int argc, char** argv) {
	int i = 7;

	int tid = UserThreadCreate(Thread, &i);
	UserThreadJoin(tid);
	i = 7;
	tid = UserThreadCreate(Thread, &i);
	UserThreadJoin(tid);

	return 0;
}

