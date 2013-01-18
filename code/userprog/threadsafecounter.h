#ifndef THREADSAFECOUNTER_H
#define	THREADSAFECOUNTER_H

#include "synch.h"

class ThreadSafeCounter : public Lock, public Condition {
public:
	ThreadSafeCounter();
	ThreadSafeCounter(const int& c);
	~ThreadSafeCounter();
	
	void operator+=(const int& i);
	void operator-=(const int& i);
	ThreadSafeCounter operator++();
	ThreadSafeCounter operator--();
	operator int();
	
	void waitUntilValue(int c);
	
private:
	int counter;
	Lock lock;
};

#endif	/* THREADSAFECOUNTER_H */

