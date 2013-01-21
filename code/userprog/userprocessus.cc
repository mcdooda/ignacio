#ifdef CHANGED
#include "userprocessus.h"
#include "machine.h"
#include "thread.h"
#include "system.h"
#include "synch.h"

class Processus {
public:

	Processus(char *name) {
		strcpy(nom, name);
		t = new Thread(nom);
	}

	Thread* getThread() {
		return t;
	}

private:
	Thread* t;
	char nom[50];

};

static Lock mutex("proc mutex");

static void StartProcessus(int arg){
	DEBUG('t',"Lancement du processus\n");
	machine->Run();
	ASSERT(FALSE);
}

int do_ForkExec(char *filename) {
	mutex.Acquire();

	OpenFile *executable = fileSystem->Open(filename);
	if (executable == NULL) {
		printf("Unable to open file %s\n", filename);
		mutex.Release();
		return -1;
	}
	IntStatus oldLevel = interrupt->SetLevel(IntOff);

	currentThread->space->SaveState();
	currentThread->SaveUserState();

	Processus *proc = new Processus(filename);
	Thread *th = proc->getThread();
	AddrSpace *space = new AddrSpace(executable);

	if (space == NULL) {
		DEBUG('t', "Mémoire insuffisante pour l'executable %s", filename);
		delete th;
		delete space;
		delete executable;
		mutex.Release();
		return -1;
	}
	delete executable; // close file
	
	th->space = space;
	th->space->InitRegisters(); // set the initial register values
	th->SaveUserState();

	th->ForkProcessus(StartProcessus, NULL);

	currentThread->RestoreUserState();
	currentThread->space->RestoreState();
	
	(void) interrupt->SetLevel(oldLevel);
	mutex.Release();
	
	return 0;
}
#endif // CHANGED