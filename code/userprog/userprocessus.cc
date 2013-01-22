#ifdef CHANGED
#include "userprocessus.h"
#include "machine.h"
#include "thread.h"
#include "system.h"
#include "synch.h"

static Lock mutex("proc mutex");

static int next_pid = 0;
static std::map<int, Processus*> processusToBeDestroyed;

extern std::map<int, Processus*> processus;

Processus::Processus(char *name) {
	strcpy(nom, name);
	t = new Thread(nom);
	nb_fils = 0;
}

Processus::Processus(Thread* th, int pid_, int ppid_){
	t = th;
	strcpy(nom,th->getName());
	pid = pid_;
	ppid = ppid_;
	nb_fils = 0;
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
	
	Processus *currentProc = processus[currentThread->getPid()];
	currentProc->AjouterFils(proc);
	
	proc->RetirerFils(currentProc);
	currentProc->RetirerFils(proc);
	
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

int Processus::AjouterFils(Processus *p){
	ajouterFils.Acquire();
	std::map<int, Processus*>::iterator it;
	std::map<int, Processus*> pfile = processus;
	
	
	next_pid++;
	p->setPid(next_pid);
	p->setPPid(this->getPid());
	
	this->fils[p->getPid()] = p;
	nbFilsPlus();
	
	DEBUG('t',"Liste des fils :");
	for (it = fils.begin(); it != fils.end(); it++) {
		DEBUG('t',"%p --",it->second->getThread());
	}
	DEBUG('t',"\n");
		
	processus[p->getPid()] = p;
	DEBUG('t',"Table des processus :\n");
	for (it = pfile.begin(); it != pfile.end(); it++) {
		DEBUG('t',"%d - Processus %p avec le thread %p : %s\n",it->first,it->second,it->second->getThread(),it->second->getThread()->getName());
	}
	
	ajouterFils.Release();
	return 0;
}

int Processus::RetirerFils(Processus *p){
	std::map<int, Processus*>::iterator it = fils.find(p->getPid());
	DEBUG('t',"%p\n",it->second);
	
	if(it->second == p){
		processusToBeDestroyed[p->getPid()] = p;		
		return 0;
	}	
	return -1;
}
#endif // CHANGED