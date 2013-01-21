#ifdef CHANGED
#include "frameprovider.h"
#include "system.h"
#include <time.h>
#include "synch.h"
static Lock mutex2("mutex bitmap");

FrameProvider::FrameProvider() :
bm(NumPhysPages) {
	RandomInit(time(NULL));
}

FrameProvider::~FrameProvider() {
}

// Retourne un numéro de frame libre
// Vérifier auparavant NumAvailFrame()
unsigned int FrameProvider::GetEmptyFrame(bool random) {
	mutex2.Acquire();
	int num;
	if (!random) {
		num = bm.Find();
	} else {
		int r = Random() % NumPhysPages;
		while (bm.Test(r))
			r = (r + 1) % NumPhysPages;
		num = r;
	}

	if (num != -1) {
		// Mise à zéro des bits de la page
		bzero(&(machine->mainMemory[num * PageSize]), PageSize);
	}
	mutex2.Release();
	return num;
}

void FrameProvider::ReleaseFrame(unsigned int num) {
	bm.Clear(num);
}

unsigned int FrameProvider::NumAvailFrame() {
	mutex2.Acquire();
	int i = bm.NumClear();
	mutex2.Release();
	return i;
}
#endif
