#ifdef CHANGED
#include "frameprovider.h"
#include "system.h"

FrameProvider::FrameProvider() :
bm(NumPhysPages) {

}

FrameProvider::~FrameProvider() {
}

unsigned int FrameProvider::GetEmptyFrame() {
	int num = bm.Find();
	if (num != -1) {
		bm.Mark(num);
		bzero(&(machine->mainMemory[num * PageSize]), PageSize);
	}
	return num;
}

void FrameProvider::ReleaseFrame(unsigned int num) {
	bm.Clear(num);
}

unsigned int FrameProvider::NumAvailFrame() {
	return bm.NumClear();
}
#endif
