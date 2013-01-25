#include "libsynch.h"
#include "syscall.h"

sem_t sem_create(const char* name, int arg){
	return SemCreate(name, arg);
}

void sem_destroy(sem_t addr){
	SemDestroy(addr);
}

void sem_P(sem_t addr){
	SemP(addr);
}

void sem_V(sem_t addr){
	SemV(addr);
}

mutex_t mutex_create(const char* name){
	return MutexCreate(name);
}

cond_t cond_create(const char* name){
	return CondCreate(name);
}

void mutex_destroy(mutex_t addr){
	MutexDestroy(addr);
}

void cond_destroy(cond_t addr){
	CondDestroy(addr);
}

void mutex_P(mutex_t addr){
	MutexP(addr);
}

void cond_wait(cond_t addr1,mutex_t addr2){
	CondWait(addr1, addr2);
}

void mutex_V(mutex_t addr){
	MutexV(addr);
}

void cond_signal(cond_t addr1,mutex_t addr2){
	CondSignal(addr1, addr2);
}

void cond_broadcast(cond_t* addr1,mutex_t addr2){
	int i = 0;
	while(addr1[i]!= 0){
		cond_signal(addr1[i],addr2);
		i++;
	}
}