// exception.cc 
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.  
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

#ifdef CHANGED
#include "synchconsole.h"
#include "usermachine.h"
#include "userthread.h"
#include "userprocessus.h"
#include "userfile.h"
#endif

//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------

static void
UpdatePC() {
	int pc = machine->ReadRegister(PCReg);
	machine->WriteRegister(PrevPCReg, pc);
	pc = machine->ReadRegister(NextPCReg);
	machine->WriteRegister(PCReg, pc);
	pc += 4;
	machine->WriteRegister(NextPCReg, pc);
}


//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!)
//
//      "which" is the kind of exception.  The list of possible exceptions 
//      are in machine.h.
//----------------------------------------------------------------------
/*
void
ExceptionHandler (ExceptionType which)
{
	int type = machine->ReadRegister (2);

	if ((which == SyscallException) && (type == SC_Halt))
	  {
		  DEBUG ('a', "Shutdown, initiated by user program.\n");
		  interrupt->Halt ();
	  }
	else
	  {
		  printf ("Unexpected user mode exception %d %d\n", which, type);
		  ASSERT (FALSE);
	  }

	// LB: Do not forget to increment the pc before returning!
	UpdatePC ();
	// End of addition
}
 */
#ifdef USER_PROGRAM
#ifdef CHANGED
extern UserMachine* userMachine;
extern SynchConsole* synchConsole;
#endif
#endif

void ExceptionHandler(ExceptionType which) {
	int type = machine->ReadRegister(2);
#ifndef CHANGED // Noter le if*n*def
	if ((which == SyscallException) && (type == SC_Halt)) {
		DEBUG('a', "Shutdown, initiated by user program.\n");
		interrupt->Halt();
	} else {
		printf("Unexpected user mode exception %d %d\n", which, type);
		ASSERT(FALSE);
	}
	UpdatePC();
#else // CHANGED
	if (which == SyscallException) {
		switch (type) {
			case SC_Halt:
			{
				DEBUG('a', "Shutdown, initiated by user program.\n");
				JoinUserThreads();
				exitProc(GetPid(currentThread));
				interrupt->Halt();
				break;
			}
			case SC_Exit:
			{
				JoinUserThreads();
				exitProc(GetPid(currentThread));
				int code = userMachine->GetIntArg(1);
				interrupt->Exit(code);
				break;
			}
			case SC_PutChar:
			{
				char c = userMachine->GetCharArg(1);
				synchConsole->SynchPutChar(c);
				break;
			}
			case SC_GetChar:
			{
				int c = synchConsole->SynchGetChar();
				userMachine->SetReturn(c);
				break;
			}
			case SC_PutString:
			{
				char str[MAX_STRING_SIZE];
				userMachine->GetStringArg(1, str);
				synchConsole->SynchPutString(str);
				break;
			}
			case SC_GetString:
			{
				int size = userMachine->GetIntArg(2);
				size = (size > MAX_STRING_SIZE ? MAX_STRING_SIZE : size);
				char strTmp[size];
				synchConsole->SynchGetString(strTmp, size);
				userMachine->SetOutArg(1, strTmp);
				break;
			}
			case SC_PutInt:
			{
				int n = userMachine->GetIntArg(1);
				char str[MAX_INTSTR_SIZE];
				snprintf(str, MAX_INTSTR_SIZE, "%d", n);
				synchConsole->SynchPutString(str);
				break;
			}
			case SC_GetInt:
			{
				char str[MAX_INTSTR_SIZE];
				synchConsole->SynchGetString(str, MAX_INTSTR_SIZE);
				int n;
				int numRead = sscanf(str, "%d", &n);
				userMachine->SetOutArg(1, n);
				userMachine->SetReturn(numRead > 0);
				break;
			}
			case SC_UserThreadCreate:
			{
				int f = userMachine->GetIntArg(1);
				int arg = userMachine->GetIntArg(2);
				int pointerExit = userMachine->GetIntArg(3);
				int id = do_UserThreadCreate(f, arg, pointerExit);
				userMachine->SetReturn(id);
				break;
			}
			case SC_UserThreadExit:
			{
				do_UserThreadExit();
				break;
			}
			case SC_UserThreadJoin:
			{
				int id = userMachine->GetIntArg(1);
				do_UserThreadJoin(id);
				break;
			}
			case SC_ForkExec:
			{
				char strTmp[MAX_STRING_SIZE];
				userMachine->GetStringArg(1, strTmp);
				int pid = do_ForkExec(strTmp);
				userMachine->SetReturn(pid);
				break;
			}
			case SC_Create:
			{
				char fileName[MAX_STRING_SIZE];
				userMachine->GetStringArg(1, fileName);
				do_Create(fileName);
				break;
			}
			case SC_Open:
			{
				char fileName[MAX_STRING_SIZE];
				userMachine->GetStringArg(1, fileName);
				int fd = do_Open(fileName);
				userMachine->SetReturn(fd);
				break;
			}
			case SC_Read:
			{
				char buf[MAX_STRING_SIZE];
				int count = userMachine->GetIntArg(2);
				int fd = userMachine->GetIntArg(3);
				int numRead = do_Read(fd, buf, count);
				userMachine->SetOutArg(1, buf);
				userMachine->SetReturn(numRead);
				break;
			}
			case SC_Write:
			{
				char buf[MAX_STRING_SIZE];
				userMachine->GetStringArg(1, buf);
				int count = userMachine->GetIntArg(2);
				int fd = userMachine->GetIntArg(3);
				int numWritten = do_Write(fd, buf, count);
				userMachine->SetReturn(numWritten);
				break;
			}
			case SC_Close:
			{
				int fd = userMachine->GetIntArg(1);
				int err = do_Close(fd);
				userMachine->SetReturn(err);
				break;
			}
			case SC_AllocEmptyPage:
			{
//				unsigned size = userMachine->GetIntArg(1);
				//TODO
				userMachine->SetReturn(0);
				break;
			}
			case SC_FreePage:
			{
//				unsigned addr = userMachine->GetIntArg(1);
				//TODO
				userMachine->SetReturn(0);
				break;
			}
			case SC_Sbrk:
			{
				unsigned nbFrames = userMachine->GetIntArg(1);
				userMachine->SetReturn(currentThread->space->Sbrk(nbFrames));
				break;
			}
			default:
			{
				printf("Unexpected user mode exception %d %d\n", which, type);
				ASSERT(FALSE);
			}
		}
		UpdatePC();
	}
#endif // CHANGED
}
