#ifdef CHANGED
#include "userprocessus.h"
#include "machine.h"
#include "thread.h"
#include "system.h"
#include "userthread.h"
#include "synchconsole.h"
#include <string>
#include "synchmap.h"
#include <iostream>
#include <set>
#include "userfile.h"

class Processus {
public:

	Processus(Thread *thread, int pid_, int ppid_, int pE, char *filename_) :
	semExit("Processus", 0),
	semStart("StartProc", 1) {
		t = thread;
		pid = pid_;
		ppid = ppid_;
		pointerExit = pE;
		filename = new char[strlen(filename_) + 1];
		strcpy(filename, filename_);
		waited = false;
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

	Semaphore* GetSemExit() {
		return &semExit;
	}

	SynchMap<int, Processus*> GetSons() {
		return sons;
	}

	void RemoveOpenFile(int id) {
		std::set<int>::iterator it = openFiles.find(id);
		openFiles.erase(it);
	}

	void AddOpenFile(int id) {
		openFiles.insert(id);
	}

	bool CheckOpenFile(int id) {
		return openFiles.find(id) != openFiles.end();
	}

	void CloseOpenFiles() {
		std::set<int> files = openFiles;
		for (std::set<int>::iterator it = files.begin(); it != files.end(); it++) {
			do_Close(pid, *it);
		}
	}

	Semaphore* GetSemStart() {
		return &semStart;
	}

	Thread* GetThread() {
		return t;
	}

	void addSonMap(Processus* procSon) {
		sons.SynchAdd(procSon->GetPid(), procSon);
	}

	char* GetFilename() {
		return filename;
	}

	bool Failed() {
		return pid == -1;
	}

	bool IsWaited() {
		return waited;
	}

	void SetWaited(bool waited_) {
		this->waited = waited_;
	}


private:
	Thread* t;
	int pid;
	int ppid;
	int pointerExit;
	Semaphore semExit;
	Semaphore semStart;
	char* filename;
	SynchMap<int, Processus*> sons;
	std::set<int> openFiles;
	bool waited;

};

static Semaphore semFinish("Finish", 1);

static int nextPid = 1;
static SynchMap<int, Processus*> processus;

extern SynchConsole *synchConsole;

static int NewPid() {
	int pid = nextPid;
	nextPid++;
	return pid;
}

static void saveSon(int pidFather, Processus* procSon) {
	processus.Get(pidFather)->addSonMap(procSon);
	processus.Add(procSon->GetPid(), procSon);
}

static Processus* GetProc(Thread* thread) {
	Processus* proc = NULL;
	const std::map<int, Processus*>& procMap = processus.GetMap();
	for (std::map<int, Processus*>::const_iterator it = procMap.begin(); it != procMap.end(); it++) {
		if (it->second->GetThread() == thread) {
			proc = it->second;
			break;
		}
	}
	return proc;
}

static int GetPid(Processus* proc) {
	int pid = -1;
	const std::map<int, Processus*>& procMap = processus.GetMap();
	for (std::map<int, Processus*>::const_iterator it = procMap.begin(); it != procMap.end(); it++) {
		if (it->second == proc) {
			pid = it->first;
			break;
		}
	}
	return pid;
}

int GetPid(Thread* t) {
	return GetPid(GetProc(t));
}

void addMainThread(Thread* m) {
	processus.P();
	char main[5] = "main";
	m->setPid(0);
	Processus* p = new Processus(m, 0, 0, -1, main);
	processus.Add(0, p);
	CreateProcessusThreadsTable(0);
	processus.V();
}

static void StartProcessus(int pid) {
	IntStatus oldLevel = interrupt->SetLevel(IntOff);
	DEBUG('t', "Lancement du processus\n");
	char* fileExec = processus.Get(pid)->GetFilename();

	OpenFile *executable = fileSystem->Open(fileExec);

	ASSERT(executable != NULL);
	//	if (executable == NULL) {
	//		printf("Unable to open file %s\n", fileExec);
	//		deleteProcessus(pid);
	//	}
	currentThread->space = new AddrSpace(executable);
	delete executable; // close file

	currentThread->space->InitRegisters(); // set the initial register values
	currentThread->space->RestoreState(); // load page table register

	(void) interrupt->SetLevel(oldLevel);
	DEBUG('t', "Fin de Lancement du processus + addr : %p\n", currentThread->space);
	machine->Run(); // jump to the user progam
	ASSERT(FALSE); // machine->Run never returns;
}

int do_ForkExec(char *filename, int pointerExit) {
	DEBUG('t', "who do fork addr : %p\n", currentThread->space);

	OpenFile *executable = fileSystem->Open(filename);
	if (executable == NULL) {
		printf("Unable to open file %s\n", filename);
		return -1;
	}
	delete executable;


	processus.P();

	int pid = NewPid();
	int ppid = GetPid(GetProc(currentThread));

	CreateProcessusThreadsTable(pid);

	Thread *t = new Thread("forked fork user");
	t->setPid(pid);
	t->setProcessus();
	Processus* p = new Processus(t, pid, ppid, pointerExit, filename);
	saveSon(ppid, p);
	t->ForkProcessus(StartProcessus, pid);

	processus.V();

	return pid;
}

void deleteProcessus(int pid) {
	Processus* p;
	if (processus.SynchTryGet(pid, p)) {
		delete p;
		processus.SynchErase(pid);
		DestroyProcessusThreadsTable(pid);
		synchConsole->ClearErr();
	}
}

void waitPid(int pid) {
	Processus* p = NULL;
	if (processus.SynchTryGet(pid, p)) {
		p->GetSemExit()->P();
		deleteProcessus(pid);
	}
}

void do_UserWaitPid(int pid) {
	waitPid(pid);
}

void exitProc(int pid) {
	JoinUserThreads(pid);
	CloseFilesProc(pid);
	Processus* p = NULL;
	if (processus.SynchTryGet(pid, p)) {
		const std::map<int, Processus*>& sonsMap = p->GetSons().GetMap();
		for (std::map<int, Processus*>::const_iterator it = sonsMap.begin(); it != sonsMap.end(); it++) {
			waitPid(it->first);
		}
		p->GetSemExit()->V();
		//DEBUG('t', "-----------------Finish du processus %s\n", processus[pid]->GetFilename());
		if (pid > 0) {
			currentThread->Finish();
		} else {
			interrupt->Halt();
		}
	}
}

void RestoreState(int pid) {
	// restauration du path

	// restauration des positions des position de curseur
}

void AddFileProc(int pid, int fd) {
	processus.P();
	Processus* p = processus.Get(pid);
	p->AddOpenFile(fd);
	processus.V();
}

void RemoveFileProc(int pid, int fd) {
	processus.P();
	Processus* p = processus.Get(pid);
	p->RemoveOpenFile(fd);
	processus.V();
}

bool CheckFileProc(int pid, int fd) {
	processus.P();
	Processus* p = processus.Get(pid);
	int ok = p->CheckOpenFile(fd);
	processus.V();
	return ok;
}

void CloseFilesProc(int pid) {
	Processus* p = processus.Get(pid);
	p->CloseOpenFiles();
}

#endif // CHANGED
