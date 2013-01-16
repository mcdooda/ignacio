#include <iostream>
#include "threadsafecounter.h"

ThreadSafeCounter::ThreadSafeCounter() :
Lock("ThreadSafeCounter"),
Condition("ThreadSafeCounter"),
lock("ThreadSafeCounter Condition Lock") {
	counter = 0;
}

ThreadSafeCounter::ThreadSafeCounter(const int& c) :
Lock("ThreadSafeCounter"),
Condition("ThreadSafeCounter"),
lock("ThreadSafeCounter Condition Lock") {
	counter = c;
}

ThreadSafeCounter::~ThreadSafeCounter() {
}

void ThreadSafeCounter::operator+=(const int& i) {
	Acquire();
	counter += i;
	Release();
	Signal(&lock);
}

void ThreadSafeCounter::operator-=(const int& i) {
	Acquire();
	counter -= i;
	Release();
	Signal(&lock);
}

ThreadSafeCounter ThreadSafeCounter::operator++() {
	Acquire();
	counter++;
	Release();
	Signal(&lock);
	return counter;
}

ThreadSafeCounter ThreadSafeCounter::operator--() {
	Acquire();
	counter--;
	Release();
	Signal(&lock);
	return counter;
}

ThreadSafeCounter::operator int() {
	return counter;
}

void ThreadSafeCounter::waitUntilValue(int c) {
	lock.Acquire();
	while (counter != c) {
		Wait(&lock);
	}
	lock.Release();
}

