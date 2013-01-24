#ifdef CHANGED


#ifndef USERTHREAD_H
#define	USERTHREAD_H

extern int do_UserThreadCreate(int pid, int f, int arg, int pointerExit);
extern void do_UserThreadExit(int pid);
extern void do_UserThreadJoin(int pid, int id);

void JoinUserThreads(int pid);

void CreateProcessusThreadsTable(int pid);
void DestroyProcessusThreadsTable(int pid);

#endif	/* USERTHREAD_H */

#endif //CHANGED