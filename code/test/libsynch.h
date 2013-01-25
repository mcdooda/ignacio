#ifndef LIBTHREAD_H
#define	LIBTHREAD_H

typedef int sem_t;

sem_t sem_create(const char* name, int);
void sem_destroy(sem_t);
void P(sem_t);
void V(sem_t);

#endif	/* LIBTHREAD_H */
