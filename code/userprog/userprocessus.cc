#ifdef CHANGED
#include "userprocessus.h"
#include "machine.h"
#include "thread.h"
#include "system.h"
#include <string>

class Processus {
public:

	Processus(Thread *thread, int pid_, int ppid_, int pE, char *filename_) :
	sem("Processus", 0),
	semStart("StartProc", 1),
	semMap("SemMap", 1) {
		t = thread;
		pid = pid_;
		ppid = ppid_;
		pointerExit = pE;
		filename = new char[strlen(filename_) + 1];
		strcpy(filename, filename_);
	}

	~Processus() {
		delete filename;
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

	Semaphore* GetSem() {
		return &sem;
	}

	std::map<int, Processus*> GetSons() {
		return sons;
	}

	Semaphore* GetSemStart() {
		return &semStart;
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
	Semaphore semStart;
	Semaphore semMap;
	char* filename;
	std::map<int, Processus*> sons;

};

static Semaphore semCreate("do_ForkExec", 1);
static Semaphore semMap("map_processus", 1);
static Semaphore semPid("nextPid", 1);
static Semaphore semProcessus("processusMap", 1);
static Semaphore semFinish("Finish", 1);



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

int GetPid(Thread* t) {
	return GetPid(GetProc(t));
}

void addMainThread(Thread* m) {
	semProcessus.P();
	char main[5] = "main";
	Processus* p = new Processus(m, 0, 0, -1, main);
	processus[0] = p;
	semProcessus.V();
}

static void StartProcessus(int pid) {
	IntStatus oldLevel = interrupt->SetLevel(IntOff);
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
	processus[pid]->GetSemStart()->V();
	(void) interrupt->SetLevel(oldLevel);
	DEBUG('t', "Fin de Lancement du processus\n");
	machine->Run(); // jump to the user progam
	ASSERT(FALSE); // machine->Run never returns;
}

int do_ForkExec(char *filename) {
	semCreate.P();
	Thread *t = new Thread("forked thread user");
	int pid = NewPid();
	int ppid = GetPid(GetProc(currentThread));

	Processus* p = new Processus(t, pid, ppid, -1, filename);
	p->GetSemStart()->P();
	saveSon(ppid, p);
	t->ForkProcessus(StartProcessus, pid);

	semCreate.V();
	return pid;
}

void deleteProcessus(int pid) {
	std::map<int, Processus*>::iterator it = processus.find(pid);
	delete it->second;
	processus.erase(it);
}

void waitPid(int pid) {
	semProcessus.P();
	std::map<int, Processus*>::iterator it = processus.find(pid);
	if (it != processus.end()) {
		Processus* son = it->second;
		semProcessus.V();
		son->GetSem()->P();
		semProcessus.P();
		deleteProcessus(pid);
		semProcessus.V();
	} else {
		semProcessus.V();
	}
}

void exitProc(int pid) {
	semProcessus.P();
	std::map<int, Processus*> procSons = processus[pid]->GetSons();
	semProcessus.V();
	for (std::map<int, Processus*>::iterator it = procSons.begin(); it != procSons.end(); it++) {
		waitPid(it->first);
	}
	semProcessus.P();
	processus[pid]->GetSem()->V();
	if (processus.size() == 1) {
		semProcessus.V();
		interrupt->Halt();
	} else {

		DEBUG('t', "-----------------Finnish du processus %s\n",processus[pid]->GetFilename());
		semProcessus.V();

		currentThread->Finish();
	}
}

#endif // CHANGED