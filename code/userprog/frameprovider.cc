#ifdef CHANGED
#include "frameprovider.h"
#include "system.h"
#include <time.h>
#include "synch.h"
static Lock mutex2("mutex FrameProvider");

FrameProvider::FrameProvider() :
bm(NumPhysPages) {
}

FrameProvider::~FrameProvider() {
}

// Retourne un numéro de frame libre
int FrameProvider::GetEmptyFrame(bool random) {
	return GetEmptyFrameSynch(random, true);
}

unsigned* FrameProvider::GetEmptyFrames(unsigned n, bool random) {
	mutex2.Acquire();
	
	if(NumAvailFrameSynch(false) < n) { // plus de frame libre
		mutex2.Release();
		return NULL;
	}
	
	unsigned *tab = new unsigned[n];
	
	for(unsigned i=0; i<n; i++)
		tab[i] = GetEmptyFrameSynch(random, false);
	
	mutex2.Release();
	return tab;
}

// Retourne un numéro de frame libre an verrouillant le mutex ou non
int FrameProvider::GetEmptyFrameSynch(bool random, bool mutexOn) {
	//TODO plusieurs stratégies
	if(mutexOn)
		mutex2.Acquire();
	
	if(NumAvailFrameSynch(false) == 0) // plus de frame libre
		return -1;
	
	int num;
	if (!random) {
		num = bm.Find();
	} else {
		int r = Random() % NumPhysPages;
		while (bm.Test(r))
			r = (r + 1) % NumPhysPages;
		num = r;
		bm.Mark(num);
	}

	if (num != -1) {
		// Mise à zéro des bits de la page
		bzero(&(machine->mainMemory[num * PageSize]), PageSize);
	}
	
	if(mutexOn)
		mutex2.Release();
	
	return num;
}

void FrameProvider::ReleaseFrame(unsigned int num) {
	bm.Clear(num);
}

unsigned int FrameProvider::NumAvailFrame() {
	return NumAvailFrameSynch(true);
}

unsigned int FrameProvider::NumAvailFrameSynch(bool mutexOn) {
	if(mutexOn)
		mutex2.Acquire();
	
	int i = bm.NumClear();
	
	if(mutexOn)
		mutex2.Release();
	return i;
}
#endif
