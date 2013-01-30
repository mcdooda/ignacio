#ifdef CHANGED

#ifndef SYNCHMAP_H
#define SYNCHMAP_H

#include <map>
#include "synch.h"

template <class T, class U> class SynchMap
{
	public:
		SynchMap();
		
		void Add(T key, U Value);
		U& Get(T key);
		bool TryGet(T key, U& value);
		void Erase(T key);
		int GetSize();
		
		void SynchAdd(T key, U value);
		U& SynchGet(T key);
		bool SynchTryGet(T key, U& value);
		void SynchErase(T key);
		int SynchGetSize();
		
		void P();
		void V();
		
		void NewSem();
		
		const std::map<T, U>& GetMap();
		void Dump();
		
	private:
		std::map<T, U> map;
		Semaphore* sem;
};

#include "synchmap.cc"

#endif

#endif


