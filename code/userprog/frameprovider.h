#ifdef CHANGED
#ifndef FRAMEPROVIDER_H
#define	FRAMEPROVIDER_H

#include "bitmap.h"

class FrameProvider {
public:
	FrameProvider();
	~FrameProvider();
	
	// permet de récupérer un cadre libre et initialisé à 0
	unsigned int GetEmptyFrame();
	// permet de libérer un cadre obtenu par GetEmptyFrame
	void ReleaseFrame(unsigned int num);
	// permet de demander combien de cadres restent disponibles
	unsigned int NumAvailFrame();
private:
	BitMap bm;
};

#endif	/* FRAMEPROVIDER_H */
#endif
