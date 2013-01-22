#ifdef CHANGED
#include "userprocessus.h"
#include "machine.h"
#include "thread.h"
#include "system.h"

static Lock mutex("proc mutex");


Processus::Processus(char *name):mutmap("mutex du map"),
	mutlist("mutex du list"), cond("Condition de wait"){
	strcpy(nom, name);
	t = new Thread(nom);
	mutcond = new Lock("mutex de la cond");
}

Processus::Processus(char *name, Thread* th, int pid_, int ppid_):mutmap("mutex du map"),
	mutlist("mutex du list"), cond("Condition de wait"){
	t = th;
	pid = pid_;
	ppid = ppid_;
	mutcond = new Lock("mutex de la cond");
}

Processus::~Processus(){
	delete t;
	delete nom;
}

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

/*Classe Processus*/
int Processus::WaitFils(int id){
	mutcond->Acquire();
	if(FilsExiste(id)){
		while(FilsExiste(id)){
			cond.Wait(mutcond);
		}
		mutcond->Release();
		return id;
	}
	mutcond->Release();
	return -1;
}
	
int Processus::Wait(){
	mutcond->Acquire();
	mutlist.Acquire();
	if(filspid.size()>0){
		mutlist.Release();
		mutmap.Acquire();
		if(fils.size()>0){
			cond.Wait(mutcond);
		}
		mutlist.Release();
		mutcond->Release();
		return 0;
	}
	mutlist.Release();
	mutcond->Release();
	return -1;	
}

bool Processus::FilsExiste(int id){
	mutlist.Acquire();
	for (std::list<int>::iterator it = filspid.begin(); it != filspid.end(); it++) {
		if (*it == id) {
			mutlist.Release();
			return true;
		}
	}
	mutlist.Release();
	return false;
}

bool Processus::FilsEnVie(int id){
	mutmap.Acquire();
	if(fils.find(id) != fils.end()){
		mutmap.Release();
		return true;
	}
	else{
		mutmap.Release();
		return false;
	}
}
#endif // CHANGED