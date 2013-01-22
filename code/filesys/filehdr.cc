// filehdr.cc 
//	Routines for managing the disk file header (in UNIX, this
//	would be called the i-node).
//
//	The file header is used to locate where on disk the 
//	file's data is stored.  We implement this as a fixed size
//	table of pointers -- each entry in the table points to the 
//	disk sector containing that portion of the file data
//	(in other words, there are no indirect or doubly indirect 
//	blocks). The table size is chosen so that the file header
//	will be just big enough to fit in one disk sector, 
//
//      Unlike in a real system, we do not keep track of file permissions, 
//	ownership, last modification date, etc., in the file header. 
//
//	A file header can be initialized in two ways:
//	   for a new file, by modifying the in-memory data structure
//	     to point to the newly allocated data blocks
//	   for a file already on disk, by reading the file header from disk
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#include "system.h"
#include "filehdr.h"
#ifdef CHANGED
#include <cmath>
#endif

//----------------------------------------------------------------------
// FileHeader::Allocate
// 	Initialize a fresh file header for a newly created file.
//	Allocate data blocks for the file out of the map of free disk blocks.
//	Return FALSE if there are not enough free blocks to accomodate
//	the new file.
//
//	"freeMap" is the bit map of free disk sectors
//	"fileSize" is the bit map of free disk sectors
//----------------------------------------------------------------------
#ifndef CHANGED

bool
FileHeader::Allocate(BitMap *freeMap, int fileSize) {
	numBytes = fileSize;
	numSectors = divRoundUp(fileSize, SectorSize);
	if (freeMap->NumClear() < numSectors)
		return FALSE; // not enough space

	for (int i = 0; i < numSectors; i++)
		dataSectors[i] = freeMap->Find();
	return TRUE;
}
#else

bool
FileHeader::Allocate(BitMap *freeMap, int fileSize) {
	numBytes = fileSize;
	numSectors = divRoundUp(fileSize, SectorSize);
	int numFileHeaders = numSectors / NumDirect;
	if (freeMap->NumClear() < numSectors + numFileHeaders)
		return FALSE; // not enough space

	unsigned int numSectorsRestants = numSectors;
	for (int i = 0; i < numFileHeaders; i++) {
		dataSectors[i] = freeMap->Find();
		FileHeader *hdr = new FileHeader;
		hdr->FetchFrom(dataSectors[i]);
		for (unsigned int j = 0; j < numSectorsRestants && j < NumDirect; j++) {
			hdr->dataSectors[j] = freeMap->Find();
		}
		hdr->WriteBack(dataSectors[i]);
		numSectorsRestants -= NumDirect;
	}
	return TRUE;

}
#endif
//----------------------------------------------------------------------
// FileHeader::Deallocate
// 	De-allocate all the space allocated for data blocks for this file.
//
//	"freeMap" is the bit map of free disk sectors
//----------------------------------------------------------------------

#ifndef CHANGED

void
FileHeader::Deallocate(BitMap *freeMap) {
	for (int i = 0; i < numSectors; i++) {
		ASSERT(freeMap->Test((int) dataSectors[i])); // ought to be marked!
		freeMap->Clear((int) dataSectors[i]);
	}
}
#else

void
FileHeader::Deallocate(BitMap *freeMap) {
	int numFileHeaders = numSectors / NumDirect;
	unsigned int numSectorsRestants = numSectors;

	for (int i = 0; i < numFileHeaders; i++) {
		FileHeader *hdr = new FileHeader;
		hdr->FetchFrom(dataSectors[i]);

		for (unsigned int j = 0; j < numSectorsRestants && j < NumDirect; j++) {
			ASSERT(freeMap->Test((int) hdr->dataSectors[j])); // ought to be marked!
			freeMap->Clear((int) hdr->dataSectors[j]);
		}
		numSectorsRestants -= NumDirect;
		freeMap->Clear((int) dataSectors[i]);
	}
}
#endif

//----------------------------------------------------------------------
// FileHeader::FetchFrom
// 	Fetch contents of file header from disk. 
//
//	"sector" is the disk sector containing the file header
//----------------------------------------------------------------------

void
FileHeader::FetchFrom(int sector) {
	synchDisk->ReadSector(sector, (char *) this);
}

//----------------------------------------------------------------------
// FileHeader::WriteBack
// 	Write the modified contents of the file header back to disk. 
//
//	"sector" is the disk sector to contain the file header
//----------------------------------------------------------------------

void
FileHeader::WriteBack(int sector) {
	synchDisk->WriteSector(sector, (char *) this);
}

//----------------------------------------------------------------------
// FileHeader::ByteToSector
// 	Return which disk sector is storing a particular byte within the file.
//      This is essentially a translation from a virtual address (the
//	offset in the file) to a physical address (the sector where the
//	data at the offset is stored).
//
//	"offset" is the location within the file of the byte in question
//----------------------------------------------------------------------
#ifndef CHANGED

int
FileHeader::ByteToSector(int offset) {
	return (dataSectors[offset / SectorSize]);
}
#else

int FileHeader::ByteToSector(int offset) {
	FileHeader *hdr = new FileHeader;
	int idSector = offset / SectorSize;

	hdr->FetchFrom(idSector / NumDirect);

	return hdr->dataSectors[idSector % NumDirect];

	//TODO A faire en fonction du nombre d'indirection quand je serai moins débile
	/*for (int i = NumIndirection; i > 0; i--) {
		//Calcul savant...
		hdr->FetchFrom(idSector / pow(NumDirect, i));
	}*/
}
#endif

//----------------------------------------------------------------------
// FileHeader::FileLength
// 	Return the number of bytes in the file.
//----------------------------------------------------------------------

int
FileHeader::FileLength() {
	return numBytes;
}

//----------------------------------------------------------------------
// FileHeader::Print
// 	Print the contents of the file header, and the contents of all
//	the data blocks pointed to by the file header.
//----------------------------------------------------------------------

void
FileHeader::Print() {
	int i, j, k;
	char *data = new char[SectorSize];

#ifdef CHANGED
	printf("FileHeader contents.  File type: %s  File size: %d.  File blocks:\n", FileHeader::GetTypeName(type), numBytes);
	for (unsigned int l = 0; l < sizeof (FileHeader); l++) {
		printf("%02x ", ((unsigned char*) this)[l]);
	}
	printf("\n");
#else
	printf("FileHeader contents.  File size: %d.  File blocks:\n", numBytes);
#endif
	for (i = 0; i < numSectors; i++)
		printf("%d ", dataSectors[i]);
	printf("\nFile contents:\n");
	for (i = k = 0; i < numSectors; i++) {
		synchDisk->ReadSector(dataSectors[i], data);
		for (j = 0; (j < SectorSize) && (k < numBytes); j++, k++) {
			if ('\040' <= data[j] && data[j] <= '\176') // isprint(data[j])
				printf("%c", data[j]);
			else
				printf("\\%x", (unsigned char) data[j]);
		}
		printf("\n");
	}
	delete [] data;
}

#ifdef CHANGED

const char* FileHeader::GetTypeName(FileHeader::FileType t) {
	switch (t) {
		case FILE:
			return "FILE";
			break;
		case DIRECTORY:
			return "DIR";
			break;
		case SYMLINK:
			return "SYMLINK";
			break;
		default:
			return "UNKNOWN";
			break;
	}
}

void FileHeader::SetType(FileHeader::FileType t) {
	type = t;
}

FileHeader::FileType FileHeader::GetType() {
	return type;
}

void FileHeader::SetLinkSector(int sector) {
	dataSectors[0] = sector;
}

int FileHeader::GetLinkSector() {
	return dataSectors[0];
}

#endif