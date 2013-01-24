// addrspace.h 
//      Data structures to keep track of executing user programs 
//      (address spaces).
//
//      For now, we don't keep any information about address spaces.
//      The user level CPU state is saved and restored in the thread
//      executing the user program (see thread.h).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef ADDRSPACE_H
#define ADDRSPACE_H

#include "copyright.h"
#include "filesys.h"

#ifdef CHANGED
#include "bitmap.h"
#include "memalloc.h"
#endif

#define UserStackSize		4096	// increase this as necessary!
#define ThreadNbPages       4
#define NbPagesStack        (UserStackSize / PageSize)
#define NbStackSlot			(NbPagesStack / ThreadNbPages)

class AddrSpace {
public:
	AddrSpace(OpenFile * executable); // Create an address space,
	// initializing it with the program
	// stored in the file "executable"
	~AddrSpace(); // De-allocate an address space

	void InitRegisters(); // Initialize user-level CPU registers,
	// before jumping to user code

	void SaveState(); // Save/restore address space-specific
	void RestoreState(); // info on a context switch 
#ifdef CHANGED
	int GetNextFreeStack();
	void FreeStackSlot(int stackBottom);

	// alloue n nouvelles frame s vides pour l’espace d’adressage,
	// valide les n pages suivantes à partir de celle désignée par brk
	// et les associe à ces cadres, incrémente brk de n
	// et retourne un pointeur vers la nouvelle zone de données (adresse virtuelle)
	int Sbrk(unsigned n);
	void* Malloc(unsigned size);
	void Free(void *addr);
#endif

private:
	TranslationEntry * pageTable; // Assume linear page table translation
	// for now!
	unsigned int numPages; // Number of pages in the virtual 

#ifdef CHANGED
	// address space
	BitMap* bm;
	unsigned int brk; // numéro de la prochaine page à allouer dynamiquement
	unsigned int brkMin; // valeur min de brk, on ne peut plus rien libérer si atteinte
	unsigned int brkMax; // valeur max de brk, on ne peut plus rien allouer si atteinte
	MemAlloc *allocateur;
private:
	void InitBitMap();
	int GetSlotAddr(int stackSlot);
#endif
};

#endif // ADDRSPACE_H
