#ifdef CHANGED
#ifndef USERSYNCH_H
#define	USERSYNCH_H


int do_SemCreate(const char* name, int arg);
void do_SemDestroy(int addr);
void do_SemP(int addr);
void do_SemV(int addr);

#endif	/* USERSYNCH_H */
#endif // CHANGED
