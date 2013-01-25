#include "libsynch.h"
#include "syscall.h"

sem_t sem_create(const char* name, int arg){
	return SemCreate(name, arg);
}

void sem_destroy(sem_t addr){
	SemDestroy(addr);
}

void P(sem_t addr){
	SemP(addr);
}

void V(sem_t addr){
	SemV(addr);
}
