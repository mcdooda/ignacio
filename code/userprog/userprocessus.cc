#ifdef CHANGED
#include "userprocessus.h"
#include "machine.h"
#include "thread.h"
#include "system.h"
#include "synch.h"
static Lock mutex("proc mutex");

void
do_ForkExec(char *filename) {
	mutex.Acquire();

	OpenFile *executable = fileSystem->Open(filename);
	if (executable == NULL) {
		printf("Unable to open file %s\n", filename);
//		return -1;
		return ;
	}
	IntStatus oldLevel = interrupt->SetLevel(IntOff);

	currentThread->space->SaveState();
	currentThread->SaveUserState();

	Thread *proc = new Thread(filename);
	AddrSpace *space = new AddrSpace(executable);
	if (space == NULL) {
		DEBUG('t', "Mémoire insuffisante pour l'executable %s", filename);
		delete proc;
		delete space;
		delete executable;
//		return -1;
		return ;
	}
	delete executable; // close file
	proc->space = space;
	proc->space->InitRegisters(); // set the initial register values
	proc->SaveUserState();

	proc->ForkProcessus(StartProcessus, NULL);

	DEBUG('t'," -------------------------------------------------- %p\n",proc);
	
	currentThread->RestoreUserState();
	currentThread->space->RestoreState();
	
	(void) interrupt->SetLevel(oldLevel);
	mutex.Release();
	
//	return 0;
}

void StartProcessus(int arg){
	DEBUG('t',"Lancement du processus\n");
	machine->Run();
	ASSERT(FALSE);
}
#endif // CHANGED