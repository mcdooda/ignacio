#ifdef CHANGED
#include "userprocessus.h"
#include "machine.h"
#include "thread.h"
#include "system.h"
#include <string>

class Processus {
public:

    Processus(Thread *thread, int pid_, int ppid_, int pE, char *filename_) :
    sem("UserThread", 0),
    semMap("StartThread", 1) {
        t = thread;
        pid = pid_;
        ppid = ppid_;
        pointerExit = pE;
        filename = filename_; //TODO caca
    }

    int GetPointerExit() {
        return pointerExit;
    }

    int GetPid() {
        return pid;
    }

    int GetPpid() {
        return ppid;
    }

    Semaphore GetSem() {
        return sem;
    }

    std::map<int, Processus*> GetSons() {
        return sons;
    }


    Semaphore GetSemMap() {
        return semMap;
    }

    Thread* GetThread() {
        return t;
    }

    void addSonMap(Processus* procSon) {
        semMap.P();
        sons[procSon->GetPid()] = procSon;
        semMap.V();
    }

    char* GetFilename() {
        return filename;
    }

    bool Failed() {
        return pid == -1;
    }

private:
    Thread* t;
    int pid;
    int ppid;
    int pointerExit;
    Semaphore sem;
    Semaphore semMap;
    char* filename;
    std::map<int, Processus*> sons;

};

static Semaphore semCreate("do_ForkExec", 1);
static Semaphore semMap("map_processus", 1);
static Semaphore semPid("nextPid", 1);
static Semaphore semProcessus("processusMap", 1);



static int nextPid = 1;
static std::map<int, Processus*> processus;

static int NewPid() {
    semPid.P();
    int pid = nextPid;
    nextPid++;
    semPid.V();
    return pid;
}

static void saveSon(int pidFather, Processus* procSon) {
    semMap.P();
    processus[pidFather]->addSonMap(procSon);
    processus[procSon->GetPid()] = procSon;
    semMap.V();
}

static Processus* GetProc(Thread* thread) {
    semProcessus.P();
    Processus* proc = NULL;
    for (std::map<int, Processus*>::iterator it = processus.begin(); it != processus.end(); it++) {
        if (it->second->GetThread() == thread) {
            proc = it->second;
            break;
        }
    }
    semProcessus.V();
    return proc;
}

static int GetPid(Processus* proc) {
    semProcessus.P();
    int pid = -1;
    for (std::map<int, Processus*>::iterator it = processus.begin(); it != processus.end(); it++) {
        if (it->second == proc) {
            pid = it->first;
            break;
        }
    }
    semProcessus.V();
    return pid;
}

int GetPid(Thread* t){
    return GetPid(GetProc(t));
}
void addMainThread(Thread* m){
    char main[5] = "main";
    Processus* p = new Processus(m, 0, 0, -1, main);
    processus[0] = p;
}
static void StartProcessus(int pid) {
    DEBUG('t', "Lancement du processus\n");
    char* fileExec = processus[pid]->GetFilename();

    OpenFile *executable = fileSystem->Open(fileExec);
    AddrSpace *space;

    if (executable == NULL) {
        printf("Unable to open file %s\n", fileExec);
        deleteProcessus(pid);
    }
    space = new AddrSpace(executable);
    currentThread->space = space;
    delete executable; // close file

    space->InitRegisters(); // set the initial register values
    space->RestoreState(); // load page table register

    machine->Run(); // jump to the user progam
    ASSERT(FALSE); // machine->Run never returns;
}

int do_ForkExec(char *filename) {
    semCreate.P();
    Thread *t = new Thread("forked thread user");
    int pid = NewPid();
    int ppid = GetPid(GetProc(currentThread));

    Processus* p = new Processus(t, pid, ppid, -1, filename);
    saveSon(ppid,p);
    t->Fork(StartProcessus, pid);

    semCreate.V();
    return pid;
}

void deleteProcessus(int pid){
    
}

void waitSon(int pid){
    Processus* son = processus[pid];
    if(son != NULL){
        son->GetSem().P();
        deleteProcessus(pid);
    }
}
void exitProc(int pid){
	std::map<int, Processus*> procSons = processus[pid]->GetSons();
	for (std::map<int, Processus*>::iterator it = procSons.begin(); it != procSons.end(); it++) {
		waitSon(it->first);
	}
    processus[GetPid(GetProc(currentThread))]->GetSem().V();
    currentThread->Finish();
}

    //	mutex.Acquire();
    //
    //	OpenFile *executable = fileSystem->Open(filename);
    //	if (executable == NULL) {
    //		printf("Unable to open file %s\n", filename);
    //		mutex.Release();
    //		return -1;
    //	}
    //	IntStatus oldLevel = interrupt->SetLevel(IntOff);
    //
    //	currentThread->space->SaveState();
    //	currentThread->SaveUserState();
    //
    //	Processus *proc = new Processus(filename);
    //	Thread *th = proc->getThread();
    //	AddrSpace *space = new AddrSpace(executable);
    //
    //	if (space == NULL) {
    //		DEBUG('t', "Mémoire insuffisante pour l'executable %s", filename);
    //		delete th;
    //		delete space;
    //		delete executable;
    //		mutex.Release();
    //		return -1;
    //	}
    //	delete executable; // close file
    //	
    //	Processus *currentProc = processus[currentThread->getPid()];
    //	currentProc->AjouterFils(proc);
    //	
    //	th->space = space;
    //	th->space->InitRegisters(); // set the initial register values
    //	th->SaveUserState();
    //
    //	th->ForkProcessus(StartProcessus, NULL);
    //
    //	currentThread->RestoreUserState();
    //	currentThread->space->RestoreState();
    //	
    //	(void) interrupt->SetLevel(oldLevel);
    //	mutex.Release();
    //	
    //	return 0;
//}
//
//int Processus::AjouterFils(Processus *p){
//	this->addFils.Acquire();
//	std::map<int, Processus*>::iterator it;
//	
//	
//	next_pid++;
//	p->setPid(next_pid);
//	p->setPPid(this->getPid());
//	p->getThread()->setPid(p->getPid());
//	
//	this->fils[p->getPid()] = p;
//	nbFilsPlus();
//	
//	DEBUG('t',"Liste des fils :");
//	for (it = fils.begin(); it != fils.end(); it++) {
//		DEBUG('t',"%p --",it->second->getThread());
//	}
//	DEBUG('t',"\n");
//	ajoutProc.Acquire();
//	processus[p->getPid()] = p;
//	this->filspid.insert(filspid.end(),p->getPid());
//	ajoutProc.Release();
//	std::map<int, Processus*> pfile = processus;
//	DEBUG('t',"Table des processus :\n");
//	for (it = pfile.begin(); it != pfile.end(); it++) {
//		DEBUG('t',"%d - Processus %p avec le thread %p : %s\n",it->first,it->second,it->second->getThread(),it->second->getThread()->getName());
//	}
//	
//	this->addFils.Release();
//	return 0;
//}
//
//int Processus::RetirerFils(Processus *p){
//	std::map<int, Processus*>::iterator it = fils.find(p->getPid());
//	DEBUG('t',"%p\n",it->second);
//	
//	if(it->second == p){
//		processusToBeDestroyed[p->getPid()] = p;		
//		return 0;
//	}	
//	return -1;
//}
//
///*Classe Processus*/
//int Processus::WaitFils(int id){
//	mutcond->Acquire();
//	if(FilsExiste(id)){
//		while(FilsExiste(id)){
//			cond.Wait(mutcond);
//		}
//		mutcond->Release();
//		return id;
//	}
//	mutcond->Release();
//	return -1;
//}
//	
//int Processus::Wait(){
//	mutcond->Acquire();
//	mutlist.Acquire();
//	DEBUG('t',"Nb fils eu: %d\n",filspid.size());
//	if(filspid.size()>0){
//		mutlist.Release();
//		mutmap.Acquire();
//		if(fils.size()>0){
//			DEBUG('t',"Wait --- \n");
//			cond.Wait(mutcond);
//		}
//		mutcond->Release();
//		return 0;
//	}
//	mutlist.Release();
//	mutcond->Release();
//	return -1;	
//}
//
//void Processus::Reveiller(){
//	cond.Signal(mutcond);
//}
//
//bool Processus::FilsExiste(int id){
//	mutlist.Acquire();
//	for (std::list<int>::iterator it = filspid.begin(); it != filspid.end(); it++) {
//		if (*it == id) {
//			mutlist.Release();
//			return true;
//		}
//	}
//	mutlist.Release();
//	return false;
//}
//
//bool Processus::FilsEnVie(int id){
//	mutmap.Acquire();
//	if(fils.find(id) != fils.end()){
//		mutmap.Release();
//		return true;
//	}
//	else{
//		mutmap.Release();
//		return false;
//	}
//
//}
//
//void Processus::Exit(){
//	DEBUG('t',"%p - %p : %s en attente de ses fils\n",this, this->getThread(), this->getThread()->getName());
//	Wait();
//	DEBUG('t',"Plus de fils\n");
//	processus[this->getPPid()]->Reveiller();
//// 	currentThread->Finish();
//// 	delete this;
//}
#endif // CHANGED