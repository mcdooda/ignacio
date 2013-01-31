// addrspace.cc 
//      Routines to manage address spaces (executing user programs).
//
//      In order to run a user program, you must:
//
//      1. link with the -N -T 0 option 
//      2. run coff2noff to convert the object file to Nachos format
//              (Nachos object code format is essentially just a simpler
//              version of the UNIX executable object code format)
//      3. load the NOFF file into the Nachos file system
//              (if you haven't implemented the file system yet, you
//              don't need to do this last step)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "addrspace.h"
#include "noff.h"
#include "usermachine.h"
#include "frameprovider.h"

#include <strings.h>		/* for bzero */

#ifdef CHANGED
#include <iostream>
// Allocation aléatoire des pages physiques (true/false)
#define STRATEGY true
extern FrameProvider *frameProvider;
#endif

//----------------------------------------------------------------------
// SwapHeader
//      Do little endian to big endian conversion on the bytes in the 
//      object file header, in case the file was generated on a little
//      endian machine, and we're now running on a big endian machine.
//----------------------------------------------------------------------

static void
SwapHeader(NoffHeader * noffH) {
	noffH->noffMagic = WordToHost(noffH->noffMagic);
	noffH->code.size = WordToHost(noffH->code.size);
	noffH->code.virtualAddr = WordToHost(noffH->code.virtualAddr);
	noffH->code.inFileAddr = WordToHost(noffH->code.inFileAddr);
	noffH->initData.size = WordToHost(noffH->initData.size);
	noffH->initData.virtualAddr = WordToHost(noffH->initData.virtualAddr);
	noffH->initData.inFileAddr = WordToHost(noffH->initData.inFileAddr);
	noffH->uninitData.size = WordToHost(noffH->uninitData.size);
	noffH->uninitData.virtualAddr =
			WordToHost(noffH->uninitData.virtualAddr);
	noffH->uninitData.inFileAddr = WordToHost(noffH->uninitData.inFileAddr);
}

#ifdef CHANGED

static void ReadAtVirtual(OpenFile *executable, int virtualaddr, int numBytes,
		int position, TranslationEntry *pageTable, unsigned numPages) {
	char buff[numBytes];
	executable->ReadAt(buff, numBytes, position);

	// sauvegarde
	TranslationEntry *pt = machine->pageTable;
	unsigned np = machine->pageTableSize;

	machine->pageTable = pageTable;
	machine->pageTableSize = numPages;

	for (int i = 0; i < numBytes; i++)
		machine->WriteMem(virtualaddr + i, 1, buff[i]);

	// restauration
	machine->pageTable = pt;
	machine->pageTableSize = np;
}

#endif

//----------------------------------------------------------------------
// AddrSpace::AddrSpace
//      Create an address space to run a user program.
//      Load the program from a file "executable", and set everything
//      up so that we can start executing user instructions.
//
//      Assumes that the object code file is in NOFF format.
//
//      First, set up the translation from program memory to physical 
//      memory.  For now, this is really simple (1:1), since we are
//      only uniprogramming, and we have a single unsegmented page table
//
//      "executable" is the file containing the object code to load into memory
//----------------------------------------------------------------------

AddrSpace::AddrSpace(OpenFile * executable) {
	NoffHeader noffH;
	unsigned int i, size;

	executable->ReadAt((char *) &noffH, sizeof (noffH), 0);
	if ((noffH.noffMagic != NOFFMAGIC) &&
			(WordToHost(noffH.noffMagic) == NOFFMAGIC))
		SwapHeader(&noffH);
	ASSERT(noffH.noffMagic == NOFFMAGIC);

#ifndef CHANGED
	// how big is address space?
	size = noffH.code.size + noffH.initData.size + noffH.uninitData.size + UserStackSize; // we need to increase the size
	// to leave room for the stack
	numPages = divRoundUp(size, PageSize);
	size = numPages * PageSize;

	ASSERT(numPages <= NumPhysPages); // check we're not trying
	// to run anything too big --
	// at least until we have
	// virtual memory
#else
	unsigned int numPagesCode, numPagesHeap, numPagesStack;
	// on placera les champs dans des pages différentes
	numPagesCode = divRoundUp(noffH.code.size + noffH.initData.size + noffH.uninitData.size, PageSize);
	numPagesHeap = divRoundUp(UserHeapSize, PageSize);
	numPagesStack = divRoundUp(UserStackSize, PageSize);

	numPages = numPagesCode + numPagesHeap + numPagesStack;
	size = numPages * PageSize;
	brk = brkMin = numPagesCode;
	brkMax = numPagesCode + numPagesHeap;

	unsigned *frames = frameProvider->GetEmptyFrames(numPagesCode + numPagesStack, STRATEGY);

	// pas assez de pages libres disponibles
	ASSERT(frames != NULL);

#endif
	DEBUG('a', "Initializing address space, num pages %d, size %d\n",
			numPages, size);

	// first, set up the translation 
	pageTable = new TranslationEntry[numPages];
	for (i = 0; i < numPages; i++) {
		pageTable[i].virtualPage = i;
#ifndef CHANGED
		pageTable[i].physicalPage = i; // for now, virtual page # = phys page #
		pageTable[i].valid = TRUE;
#else
		if (i < brkMin || i >= brkMax) { // TEXT, DATA ou STACK
			pageTable[i].physicalPage = frames[(i < brkMin) ? i : i - numPagesHeap];
			pageTable[i].valid = TRUE;
		} else { // HEAP
			// si la page appartient au tas
			pageTable[i].physicalPage = -1;
			pageTable[i].valid = FALSE;
		}
#endif
		pageTable[i].use = FALSE;
		pageTable[i].dirty = FALSE;
		pageTable[i].readOnly = FALSE; // if the code segment was entirely on 
		// a separate page, we could set its 
		// pages to be read-only
	}

#ifdef CHANGED
	delete frames;
#else
	// zero out the entire address space, to zero the unitialized data segment 
	// and the stack segment
	bzero(machine->mainMemory, size);
#endif
	// then, copy in the code and data segments into memory
	if (noffH.code.size > 0) {
		DEBUG('a', "Initializing code segment, at 0x%x, size %d\n",
				noffH.code.virtualAddr, noffH.code.size);
#ifndef CHANGED
		executable->ReadAt(&(machine->mainMemory[noffH.code.virtualAddr]),
				noffH.code.size, noffH.code.inFileAddr);
#else
		ReadAtVirtual(executable, noffH.code.virtualAddr, noffH.code.size,
				noffH.code.inFileAddr, pageTable, numPages);
#endif
	}
	if (noffH.initData.size > 0) {
		DEBUG('a', "Initializing data segment, at 0x%x, size %d\n",
				noffH.initData.virtualAddr, noffH.initData.size);
#ifndef CHANGED
		executable->ReadAt(&
				(machine->mainMemory
				[noffH.initData.virtualAddr]),
				noffH.initData.size, noffH.initData.inFileAddr);
#else
		ReadAtVirtual(executable, noffH.initData.virtualAddr, noffH.initData.size,
				noffH.initData.inFileAddr, pageTable, numPages);
		
		// TEXT en lecture seule (uniquement pages qui ne contiennent pas de DATA également)
		for (i = 0; i < (unsigned)divRoundDown(noffH.code.size, PageSize); i++)
			pageTable[i].readOnly = TRUE;
#endif
	}

#ifdef CHANGED
	InitBitMap();
#endif
}

//----------------------------------------------------------------------
// AddrSpace::~AddrSpace
//      Dealloate an address space.
//----------------------------------------------------------------------

AddrSpace::~AddrSpace() {
#ifdef CHANGED
	delete bm;
	for (unsigned int i = 0; i < numPages; i++)
		if (pageTable[i].valid)
			frameProvider->ReleaseFrame(pageTable[i].physicalPage);
#endif
	// LB: Missing [] for delete
	// delete pageTable;
	delete [] pageTable;
	// End of modification
}

//----------------------------------------------------------------------
// AddrSpace::InitRegisters
//      Set the initial values for the user-level register set.
//
//      We write these directly into the "machine" registers, so
//      that we can immediately jump to user code.  Note that these
//      will be saved/restored into the currentThread->userRegisters
//      when this thread is context switched out.
//----------------------------------------------------------------------

void
AddrSpace::InitRegisters() {
	int i;

	for (i = 0; i < NumTotalRegs; i++)
		machine->WriteRegister(i, 0);

	// Initial program counter -- must be location of "Start"
	machine->WriteRegister(PCReg, 0);

	// Need to also tell MIPS where next instruction is, because
	// of branch delay possibility
	machine->WriteRegister(NextPCReg, 4);

	// Set the stack register to the end of the address space, where we
	// allocated the stack; but subtract off a bit, to make sure we don't
	// accidentally reference off the end!
	machine->WriteRegister(StackReg, numPages * PageSize - 16);
	DEBUG('a', "Initializing stack register to %d\n",
			numPages * PageSize - 16);
}

//----------------------------------------------------------------------
// AddrSpace::SaveState
//      On a context switch, save any machine state, specific
//      to this address space, that needs saving.
//
//      For now, nothing!
//----------------------------------------------------------------------

void
AddrSpace::SaveState() {
	//	ASSERT(machine->pageTable != NULL);
	pageTable = machine->pageTable;
	numPages = machine->pageTableSize;
}

//----------------------------------------------------------------------
// AddrSpace::RestoreState
//      On a context switch, restore the machine state so that
//      this address space can run.
//
//      For now, tell the machine where to find the page table.
//----------------------------------------------------------------------

void AddrSpace::RestoreState() {
	//	ASSERT(pageTable != NULL);
	machine->pageTable = pageTable;
	machine->pageTableSize = numPages;
}

#ifdef CHANGED

void AddrSpace::InitBitMap() {
	bm = new BitMap(NbStackSlot);
	bm->Mark(NbStackSlot - 1);
}

int AddrSpace::GetSlotAddr(int stackSlot) {
	return (numPages * PageSize) - ((stackSlot + 1) * (PageSize * ThreadNbPages));
}

int AddrSpace::GetNextFreeStack() {
	int addr = -1;
	for (int i = 1; addr == -1 && i <= NbStackSlot; i++) {
		int currentSlot = NbStackSlot - i;
		if (!bm->Test(currentSlot)) {
			bm->Mark(currentSlot);
			addr = GetSlotAddr(currentSlot);
		}
	}
	return addr;
}

void AddrSpace::FreeStackSlot(int stackBottom) {
	//	for (int i = stackBottom; i > (stackBottom + (PageSize * ThreadNbPages)); i -= 4)
	//		machine->WriteMem(i, 4, 0);
	//	for(unsigned int i=(numPages * PageSize); i > (numPages * PageSize) - ((8 + 1) * (PageSize * ThreadNbPages)) ; i-=4){
	//		int j;
	//		machine->ReadMem(i,4,&j);
	//		printf("%d.",j);
	//	}
	//	std::cout << "stackBottom = " << stackBottom << " stackslot = " << (NbStackSlot - 1) - (numPages - (stackBottom / PageSize)) / ThreadNbPages << std::endl;
	bm->Clear((NbStackSlot - 1) - (numPages - (stackBottom / PageSize)) / ThreadNbPages);
}

int AddrSpace::Sbrk(unsigned nbFrames) {
	
	if (nbFrames == 0)
		return brk;
	
	if (brk + nbFrames > brkMax) // tas plein
		return NULL;

	unsigned *frames = frameProvider->GetEmptyFrames(nbFrames, STRATEGY);
	if (frames == NULL) // pas assez de frames libres
		return NULL;

	for (unsigned i = 0; i < nbFrames; i++) {
		pageTable[brk + i].physicalPage = frames[i];
		pageTable[brk + i].valid = TRUE;
		pageTable[brk + i].use = FALSE;
		pageTable[brk + i].dirty = FALSE;
		pageTable[brk + i].readOnly = FALSE;
	}

	delete frames;

	int address = pageTable[brk].virtualPage * PageSize;
	brk += nbFrames;
	return address;
}

int AddrSpace::AllocEmptyPage() {
	unsigned i = brkMin;
	while (i < brk && pageTable[i].valid)
		i++;

	// tas plein, il faut décaler brk
	if (i >= brk)
		return Sbrk(1);

	// allocation d'une page physique
	int frame = frameProvider->GetEmptyFrame(STRATEGY);
	if (frame == -1) // pas assez de frames libres
		return NULL;

	pageTable[i].physicalPage = frame;
	pageTable[i].valid = TRUE;
	pageTable[i].use = FALSE;
	pageTable[i].dirty = FALSE;
	pageTable[i].readOnly = FALSE;

	return i * PageSize;
}

void AddrSpace::FreePage(int addr) {
	unsigned numPage = divRoundDown(addr, PageSize);

	if (numPage >= brkMin && numPage < brk) {
		pageTable[numPage].physicalPage = -1;
		pageTable[numPage].valid = FALSE;
	}
	//TODO décrémenter BRK ?
}

#endif
