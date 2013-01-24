// progtest.cc 
//      Test routines for demonstrating that Nachos can load
//      a user program and execute it.  
//
//      Also, routines for testing the Console hardware device.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "filesys.h"
#include "console.h"
#include "synchconsole.h"
#include "addrspace.h"
#include "synch.h"
#include "syscall.h"
#ifdef CHANGED
#include "userprocessus.h"
#endif

//----------------------------------------------------------------------
// StartProcess
//      Run a user program.  Open the executable, load it into
//      memory, and jump to it.
//----------------------------------------------------------------------

void
StartProcess(char *filename) {
	OpenFile *executable = fileSystem->Open(filename);
	AddrSpace *space;

	if (executable == NULL) {
		printf("Unable to open file %s\n", filename);
		return;
	}
	space = new AddrSpace(executable);
	currentThread->space = space;

	delete executable; // close file

	space->InitRegisters(); // set the initial register values
	space->RestoreState(); // load page table register

	machine->Run(); // jump to the user progam
	ASSERT(FALSE); // machine->Run never returns;
	// the address space exits
	// by doing the syscall "exit"
}

// Data structures needed for the console test.  Threads making
// I/O requests wait on a Semaphore to delay until the I/O completes.
static Semaphore *readAvail;
static Semaphore *writeDone;
#ifdef CHANGED
extern SynchConsole* synchConsole;
extern FileSystem* fileSystem;
#endif
//----------------------------------------------------------------------
// ConsoleInterruptHandlers
//      Wake up the thread that requested the I/O.
//----------------------------------------------------------------------

static Console *console;

static void
ReadAvail(int arg) {
	readAvail->V();
}

static void
WriteDone(int arg) {
	writeDone->V();
}
//----------------------------------------------------------------------
// ConsoleTest
//      Test the console by echoing characters typed at the input onto
//      the output.  Stop when the user types a 'q'.
//----------------------------------------------------------------------

void
ConsoleTest(char *in, char *out) {
	char ch;

#ifdef CHANGED
	delete synchConsole;
	synchConsole = NULL;
#endif

	console = new Console(in, out, ReadAvail, WriteDone, 0);
	readAvail = new Semaphore("read avail", 0);
	writeDone = new Semaphore("write done", 0);
#ifndef CHANGED
	for (;;) {
		readAvail->P(); // wait for character to arrive
		ch = console->GetChar();
		console->PutChar(ch); // echo it!
		writeDone->P(); // wait for write to finish
		if (ch == 'q')
			return; // if q, quit
	}
#else
	for (;;) {
		readAvail->P(); // wait for character to arrive
		ch = console->GetChar();
		if (ch != EOF) {
			console->PutChar('<');
			writeDone->P();
			console->PutChar(ch); // echo it!
			writeDone->P();
			console->PutChar('>');
			writeDone->P(); // wait for write to finish
		}
		if (ch == 'q' || ch == EOF)
			return; // if q or EOF, quit
	}
#endif
}

#ifdef CHANGED

void SynchConsoleTest(char *in, char *out) {
	int ch;
	if (in != NULL && out != NULL) {
		delete synchConsole;
		synchConsole = new SynchConsole(in, out);
	}
	while ((ch = synchConsole->SynchGetChar()) != EOF)
		synchConsole->SynchPutChar(ch);
	// 2ème façon de vérifier si EOF
	//	while(!synchConsole->feof()) {
	//		ch = synchConsole->SynchGetChar();
	//		synchConsole->SynchPutChar(ch);
	//	}
}
extern void Copy(const char *unixFile, const char *nachosFile);

#ifdef FILESYS

void FileSystemTest() {
	Copy("../filesys/test/small", "small");

	fileSystem->CreateDirectory("Mdir");
	fileSystem->SetDirectory("Mdir/");
	Copy("../filesys/test/small", "medium");
	fileSystem->CreateDirectory("Bdir");
	fileSystem->SetDirectory("Bdir/");
	Copy("../filesys/test/small", "big");

	fileSystem->SetDirectory("../");
	fileSystem->SetDirectory("../");

	fileSystem->CreateDirectory("Vdir");
	fileSystem->SetDirectory("Vdir/");
	Copy("../filesys/test/small", "small");
	fileSystem->SetDirectory("../");

	fileSystem->MinimalisticPrint();
}

void ConsoleUser() {

	//	Copy("../filesys/test/small", "halt");

	Copy("../build/halt", "halt");
	Copy("../build/console", "console");
	Copy("../build/putstring", "putstring");
	Copy("../build/putchar", "putchar");
	fileSystem->MinimalisticPrint();

	char* cons = (char*) "console";
	StartProcess(cons);


	//	OpenFile* of = fileSystem->Open("halt");








	//	fileSystem->Print();
	//	char buf[100];
	//	if (of != NULL) {
	//		printf("fichier ouvert\n");
	//		of->Read(buf, 50);
	//		printf("%s",buf);
	//	} else {
	//		printf("fichier non ouvert\n");
	//	}

}
#endif
#endif //CHANGED

