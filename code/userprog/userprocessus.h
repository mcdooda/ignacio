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

void RestoreState(int pid);

void AddFileProc(int pid, int fd);
void RemoveFileProc(int pid, int fd);
bool CheckFileProc(int pid, int fd);
void CloseFilesProc(int pid);

#endif	/* USERPROCESSUS_H */
#endif // CHANGED
