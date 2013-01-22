#ifdef CHANGED
#include "userprocessus.h"
#include "machine.h"
#include "thread.h"
#include "system.h"

static Lock mutex("proc mutex");
static Lock ajoutProc("Ajout d'un processus");

static int next_pid = 0;
static std::map<int, Processus*> processusToBeDestroyed;

extern std::map<int, Processus*> processus;

Processus::Processus(char *name):addFils("mutex ajout fils"),mutmap("mutex du map"),
	mutlist("mutex du list"), cond("Condition de wait"){
	strcpy(nom, name);
	t = new Thread(nom);
	mutcond = new Lock("mutex de la cond");
	nb_fils = 0;
}

Processus::Processus(Thread* th, int pid_, int ppid_):addFils("mutex ajout fils"),mutmap("mutex du map"),
	mutlist("mutex du list"), cond("Condition de wait"){
	mutcond = new Lock("mutex de la cond");
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
	this->addFils.Acquire();
	std::map<int, Processus*>::iterator it;
	
	
	next_pid++;
	p->setPid(next_pid);
	p->setPPid(this->getPid());
	p->getThread()->setPid(p->getPid());
	
	this->fils[p->getPid()] = p;
	nbFilsPlus();
	
	DEBUG('t',"Liste des fils :");
	for (it = fils.begin(); it != fils.end(); it++) {
		DEBUG('t',"%p --",it->second->getThread());
	}
	DEBUG('t',"\n");
	ajoutProc.Acquire();
	processus[p->getPid()] = p;
	this->filspid.insert(filspid.end(),p->getPid());
	ajoutProc.Release();
	std::map<int, Processus*> pfile = processus;
	DEBUG('t',"Table des processus :\n");
	for (it = pfile.begin(); it != pfile.end(); it++) {
		DEBUG('t',"%d - Processus %p avec le thread %p : %s\n",it->first,it->second,it->second->getThread(),it->second->getThread()->getName());
	}
	
	this->addFils.Release();
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
	DEBUG('t',"Nb fils eu: %d\n",filspid.size());
	if(filspid.size()>0){
		mutlist.Release();
		mutmap.Acquire();
		if(fils.size()>0){
			DEBUG('t',"Wait --- \n");
			cond.Wait(mutcond);
		}
		mutcond->Release();
		return 0;
	}
	mutlist.Release();
	mutcond->Release();
	return -1;	
}

void Processus::Reveiller(){
	cond.Signal(mutcond);
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

void Processus::Exit(){
	DEBUG('t',"%p - %p : %s en attente de ses fils\n",this, this->getThread(), this->getThread()->getName());
	Wait();
	DEBUG('t',"Plus de fils\n");
	processus[this->getPPid()]->Reveiller();
// 	currentThread->Finish();
// 	delete this;
}
#endif // CHANGED