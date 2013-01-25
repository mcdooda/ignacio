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

int do_MutexCreate(const char* name){
	Lock* mut = new Lock(name);
	return (int)mut;
}

int do_CondCreate(const char* name){
	Condition* cond = new Condition(name);
	return (int)cond;
}

void do_MutexDestroy(int addr){
	delete (Lock*)addr;
}

void do_CondDestroy(int addr){
	delete (Condition*)addr;
}

void do_MutexP(int addr){
	((Lock*)addr)->Acquire();
}

void do_CondWait(int addr1,int addr2){
	((Condition*)addr1)->Wait((Lock*)addr2);
}

void do_MutexV(int addr){
	((Lock*)addr)->Release();
}

void do_CondSignal(int addr1,int addr2){
	((Condition*)addr1)->Signal((Lock*)addr2);
}

//void do_CondBroadcast(int* addr1,int addr2){
//	
//}
#endif
