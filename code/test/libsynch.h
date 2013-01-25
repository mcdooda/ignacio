#ifndef LIBTHREAD_H
#define	LIBTHREAD_H

typedef int sem_t;
typedef int mutex_t;
typedef int cond_t;

sem_t sem_create(const char* name, int);
mutex_t mutex_create(const char* name);
cond_t cond_create(const char* name);
void sem_destroy(sem_t);
void mutex_destroy(mutex_t);
void cond_destroy(cond_t);
void sem_P(sem_t);
void mutex_P(mutex_t);
void cond_wait(cond_t,mutex_t);
void sem_V(sem_t);
void mutex_V(mutex_t);
void cond_signal(cond_t,mutex_t);
void cond_broadcast(cond_t*,mutex_t);

#endif	/* LIBTHREAD_H */
