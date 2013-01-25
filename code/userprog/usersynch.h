#ifdef CHANGED
#ifndef USERSYNCH_H
#define	USERSYNCH_H


int do_SemCreate(const char* name, int arg);
void do_SemDestroy(int addr);
void do_SemP(int addr);
void do_SemV(int addr);
int do_MutexCreate(const char* name);
int do_CondCreate(const char* name);
void do_MutexDestroy(int addr);
void do_CondDestroy(int addr);
void do_MutexP(int addr);
void do_CondWait(int addr,int addr2);
void do_MutexV(int addr);
void do_CondSignal(int addr,int addr2);
//void do_CondBroadcast(int* addr,int addr2);
#endif	/* USERSYNCH_H */
#endif // CHANGED
