#ifdef CHANGED
#ifndef USERPROCESSUS_H
#define	USERPROCESSUS_H
#include "thread.h"
#include "synch.h"
#include <map>
#include <list>

void deleteProcessus(int pid);
extern int do_ForkExec(char *filename, int pointerExit);
extern void do_UserWaitPid(int pid);

extern int GetPid(Thread* t);
extern void exitProc(int pid);
extern void addMainThread(Thread* m);
#endif	/* USERPROCESSUS_H */
#endif // CHANGED
