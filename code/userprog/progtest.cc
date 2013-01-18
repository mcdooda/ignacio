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
#include "console.h"
#include "synchconsole.h"
#include "addrspace.h"
#include "synch.h"

#ifdef CHANGED
static Lock *lock = new Lock("progtest");
#endif
//----------------------------------------------------------------------
// StartProcess
//      Run a user program.  Open the executable, load it into
//      memory, and jump to it.
//----------------------------------------------------------------------

void
StartProcess (char *filename)
{
    OpenFile *executable = fileSystem->Open (filename);
    AddrSpace *space;

    if (executable == NULL)
      {
	  printf ("Unable to open file %s\n", filename);
	  return;
      }
    space = new AddrSpace (executable);
    currentThread->space = space;

    delete executable;		// close file

    space->InitRegisters ();	// set the initial register values
    space->RestoreState ();	// load page table register

    machine->Run ();		// jump to the user progam
    ASSERT (FALSE);		// machine->Run never returns;
    // the address space exits
    // by doing the syscall "exit"
}

// Data structures needed for the console test.  Threads making
// I/O requests wait on a Semaphore to delay until the I/O completes.
static Semaphore *readAvail;
static Semaphore *writeDone;
#ifdef CHANGED
extern SynchConsole* synchConsole;
#endif
//----------------------------------------------------------------------
// ConsoleInterruptHandlers
//      Wake up the thread that requested the I/O.
//----------------------------------------------------------------------

static Console *console;
static void
ReadAvail (int arg)
{
    readAvail->V ();
}
static void
WriteDone (int arg)
{
    writeDone->V ();
}
//----------------------------------------------------------------------
// ConsoleTest
//      Test the console by echoing characters typed at the input onto
//      the output.  Stop when the user types a 'q'.
//----------------------------------------------------------------------

void
ConsoleTest (char *in, char *out)
{
    char ch;
	
#ifdef CHANGED
	delete synchConsole;
	synchConsole = NULL;
#endif
	
    console = new Console (in, out, ReadAvail, WriteDone, 0);
    readAvail = new Semaphore ("read avail", 0);
    writeDone = new Semaphore ("write done", 0);
#ifndef CHANGED
	for (;;)
	{
	  readAvail->P ();	// wait for character to arrive
	  ch = console->GetChar ();
	  console->PutChar (ch);	// echo it!
	  writeDone->P ();	// wait for write to finish
	  if (ch == 'q')
		return;		// if q, quit
	}
#else
    for (;;)
    {
	  readAvail->P ();	// wait for character to arrive
	  ch = console->GetChar ();
	  if(ch != EOF)
	  {
		console->PutChar ('<');
		writeDone->P ();
		console->PutChar (ch);	// echo it!
		writeDone->P ();
		console->PutChar ('>');
		writeDone->P ();	// wait for write to finish
	  }
	  if (ch == 'q' || ch == EOF)
		return;		// if q or EOF, quit
    }
#endif
}

#ifdef CHANGED
void SynchConsoleTest (char *in, char *out)
{
	int ch;
	if(in != NULL && out != NULL) {
		delete synchConsole;
		synchConsole = new SynchConsole(in, out);
	}
	while((ch = synchConsole->SynchGetChar()) != EOF)
		synchConsole->SynchPutChar(ch);
	// 2ème façon de vérifier si EOF
//	while(!synchConsole->feof()) {
//		ch = synchConsole->SynchGetChar();
//		synchConsole->SynchPutChar(ch);
//	}
}

//----------------------------------------------------------------------
// StartProcessExec
//      Run a user program.  Open the executable, load it into a new memory space
//      , and jump to it.
//----------------------------------------------------------------------

static void StartProcExec(int id=0) {
	machine->Run();
	ASSERT(false);
}

int StartProcessExec(char *filename) {
	lock->Acquire();
	
	OpenFile *executable = fileSystem->Open(filename);
	if (executable == NULL) {
		printf("Unable to open file %s\n", filename);
		return -1;
	}
	IntStatus oldLevel = interrupt->SetLevel (IntOff);
	
	currentThread->space->SaveState();
	currentThread->SaveUserState();
	
	AddrSpace *space = new AddrSpace(executable);
	delete executable; // close file
	Thread *t = new Thread(filename);

	t->space = space;
	t->space->InitRegisters(); // set the initial register values
//	t->space->RestoreState(); // load page table register
	t->SaveUserState();
	t->ForkExec(StartProcExec, 0);
//	t->ForkExec((VoidFunctionPtr)(t->space->pageTable[t->space->noffH.code.virtualAddr].physicalPage*PageSize),0);
	
	currentThread->RestoreUserState();
	currentThread->space->RestoreState();

	interrupt->SetLevel (oldLevel);
	lock->Release();
	
	return 0;
}
#endif //CHANGED

