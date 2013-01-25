#ifdef CHANGED
#include "usersynch.h"
#include "synch.h"

int do_SemCreate(const char* name, int arg){
	Semaphore* sem = new Semaphore(name, arg);
	return (int)sem;
}

void do_SemDestroy(int addr){
	delete (Semaphore*)addr;
}

void do_SemP(int addr){
	((Semaphore*)addr)->P();
}
void do_SemV(int addr){
	((Semaphore*)addr)->V();
}

#endif
