#ifdef CHANGED
#ifndef FRAMEPROVIDER_H
#define	FRAMEPROVIDER_H

#include "bitmap.h"

class FrameProvider {
public:
	FrameProvider();
	~FrameProvider();
	
	// permet de récupérer un cadre libre et initialisé à 0
	// retourne -1 si pas assez de frames
	int GetEmptyFrame(bool random = false);
	// permet de récupérer plusieurs cadres libres et initialisés à 0
	// retourne NULL si pas assez de frames
	unsigned* GetEmptyFrames(unsigned n, bool random = false);
	// permet de libérer un cadre obtenu par GetEmptyFrame
	void ReleaseFrame(unsigned int num);
	// permet de demander combien de cadres restent disponibles
	unsigned int NumAvailFrame();
private:
	int GetEmptyFrameSynch(bool random, bool mutex);
	unsigned int NumAvailFrameSynch(bool mutexOn);
	BitMap bm;
};

#endif	/* FRAMEPROVIDER_H */
#endif
