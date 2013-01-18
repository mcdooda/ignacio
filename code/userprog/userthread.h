#ifdef CHANGED


#ifndef USERTHREAD_H
#define	USERTHREAD_H

extern int do_UserThreadCreate(int f, int arg, int pointerExit);
extern void do_UserThreadExit();
extern void do_UserThreadJoin(int id);

void JoinUserThreads();

#endif	/* USERTHREAD_H */

#endif //CHANGED