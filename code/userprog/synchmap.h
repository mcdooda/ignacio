#ifdef CHANGED

#ifndef SYNCHMAP_H
#define SYNCHMAP_H

#include <map>
#include "synch.h"

template <class T, class U> class SynchMap : public std::map<T, U>
{
	public:
		SynchMap();
		
		bool TryGet(T key, U& value);
		void Erase(T key);
		
		void SynchAdd(T key, U value);
		const U& SynchGet(T key);
		bool SynchTryGet(T key, U& value);
		void SynchErase(T key);
		
		void P();
		void V();
		
	private:
		Semaphore sem;
};

#include "synchmap.cc"

#endif

#endif


