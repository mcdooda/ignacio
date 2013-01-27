#ifdef CHANGED

#include "userthread.h"
#include "machine.h"
#include "thread.h"
#include "system.h"
#include "synch.h"
#include "threadsafecounter.h"
#include "userprocessus.h"
#include <map>
#include <iostream>
#include <string>
#include <sstream>
//#include <stdlib>
#include "synchmap.h"

static int NewThreadId();

class UserThread {
public:

	UserThread(int function, int argument, int pE, Thread* thread) :
	sem("UserThread", 0),
	semStart("StartThread", 0) {
		f = function;
		arg = argument;
		pointerExit = pE;
		t = thread;
		id = NewThreadId();

	}

	int GetPointerExit() {
		return pointerExit;
	}

	int GetArg() {
		return arg;
	}

	int GetF() {
		return f;
	}

	int GetId() {
		return id;
	}

	Semaphore* GetSem() {
		return &sem;
	}

	Semaphore* GetSemStart() {
		return &semStart;
	}

	Thread* GetThread() {
		return t;
	}

	int GetStackBottom() {
		return stackBottom;
	}

	void SetStackBottom(int sb) {
		stackBottom = sb;
	}

	bool Failed() {
		return stackBottom == -1;
	}

private:
	int f;
	int arg;
	int pointerExit;
	int id;
	int stackBottom;
	Semaphore sem;
	Semaphore semStart;
	Thread* t;

};

static Semaphore semCreate("do_UserThreadCreate", 1);
static Semaphore semFreeStack("GetFreeStackSpace", 1);
static Semaphore semJoin("UserThreadJoin", 1);
static ThreadSafeCounter threadCounter(0);

static int nextTid = 1;
static SynchMap<int, SynchMap<int, UserThread*>* > userThreads;
static SynchMap<int, ThreadSafeCounter*> threadsCounters;

static int NewThreadId() {
	int tid = nextTid;
	nextTid++;
	return tid;
}

static int SaveUserThread(int pid, UserThread* ut) {
	userThreads.SynchGet(pid)->SynchAdd(ut->GetId(), ut);
	return ut->GetId();
}

static void DeleteUserThread(int pid, UserThread* ut) {
	SynchMap<int, UserThread*>* procThreads = userThreads.SynchGet(pid);
	procThreads->SynchErase(ut->GetId());
	delete ut;
}

static UserThread* GetUserThread(int pid, Thread* thread) {
	userThreads.P();
	SynchMap<int, UserThread*>* procThreads = userThreads[pid];
	UserThread* ut = NULL;
	for (SynchMap<int, UserThread*>::iterator it = procThreads->begin(); it != procThreads->end(); it++) {
		if (it->second->GetThread() == thread) {
			ut = it->second;
			break;
		}
	}
	userThreads.V();
	return ut;
}

static UserThread* GetUserThread(int pid, int id) {
	userThreads.P();

	//	std::cout << "Table des Threads ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
	//	for (std::map<int, std::map<int, UserThread*>*>::iterator it = userThreads.begin(); it != userThreads.end(); it++) {
	//		std::cout << "pid = " << it->first << std::endl;
	//		std::map<int, UserThread*>* t = it->second;
	//		for (std::map<int, UserThread*>::iterator it2 = t->begin(); it2 != t->end(); it2++) {
	//			std::cout << "\ttid = " << it2->first << std::endl;
	//		}
	//	}
	//	std::cout << "-----------------------" << std::endl;

	SynchMap<int, UserThread*>* procThreads = userThreads[pid];
	UserThread* ut = NULL;
	procThreads->TryGet(id, ut);
	userThreads.V();
	return ut;
}

static void StartUserThread(int id) {
	int freeStackPointer;
	int pid = currentThread->getPid();
	UserThread* ut = GetUserThread(pid, id);
	ut->GetThread()->space->InitRegisters();
	machine->WriteRegister(4, ut->GetArg());
	machine->WriteRegister(PCReg, ut->GetF());
	machine->WriteRegister(NextPCReg, ut->GetF() + 4);
	machine->WriteRegister(RetAddrReg, ut->GetPointerExit());

	semFreeStack.P();
	freeStackPointer = currentThread->space->GetNextFreeStack();
	ut->SetStackBottom(freeStackPointer);
	semFreeStack.V();
	ut->GetSemStart()->V();
	if (freeStackPointer != -1) {
		machine->WriteRegister(StackReg, freeStackPointer);
		machine->Run();
	} else {
		std::cout << "creation du thread impossible" << std::endl;
		do_UserThreadExit(pid);
	}
}

int do_UserThreadCreate(int pid, int f, int arg, int pE) {
	semCreate.P(); //TODO: verifier la safe-threadry
	std::stringstream ss;
	ss << "forked thread user ";
	ss << pid;
	std::string bite = ss.str();
	char* str = new char[bite.size() + 1];
	strcpy(str, bite.c_str());
	Thread *t = new Thread(str);
	UserThread* ut = new UserThread(f, arg, pE, t);
	t->setPid(pid);
	int id = SaveUserThread(pid, ut);
	t->Fork(StartUserThread, id);
	ut->GetSemStart()->P();
	if (ut->Failed()) {
		id = -1;
		DeleteUserThread(pid, ut);
	} else {
		//TODO inutile ?
		t->space = currentThread->space;
		//
		threadsCounters.P();
		++(*threadsCounters[pid]);
		threadsCounters.V();

	}
	semCreate.V();
	return id;
}

void do_UserThreadExit(int pid) {
//	std::cout << "do_userthread exit " << pid << std::endl;
//	std::cout << "Table des Threads ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
//	for (std::map<int, std::map<int, UserThread*>*>::iterator it = userThreads.begin(); it != userThreads.end(); it++) {
//		std::cout << "pid = " << it->first << std::endl;
//		std::map<int, UserThread*>* t = it->second;
//		for (std::map<int, UserThread*>::iterator it2 = t->begin(); it2 != t->end(); it2++) {
//			std::cout << "\ttid = " << it2->first << std::endl;
//		}
//	}
//	std::cout << "-----------------------" << std::endl;
	
	UserThread* ut = GetUserThread(pid, currentThread);
	ut->GetThread()->space->FreeStackSlot(ut->GetStackBottom());

	ut->GetSem()->V();
	threadsCounters.P();
	--(*threadsCounters[pid]);
	threadsCounters.V();
	currentThread->Finish();
}

void do_UserThreadJoin(int pid, int id) {
	UserThread* ut = GetUserThread(pid, id); // thread safe!
	// pas thread safe!
	if (ut != NULL) {
		ut->GetSem()->P();
		int i;
		machine->ReadMem(ut->GetArg(), 4, &i);
		DeleteUserThread(pid, ut);
	}
}

void JoinUserThreads(int pid) {
	//	threadCounter.waitUntilValue(0);
	threadsCounters[pid]->waitUntilValue(0);
	std::map<int, UserThread*> procThreads = *userThreads[pid];
	for (std::map<int, UserThread*>::iterator it = procThreads.begin(); it != procThreads.end(); it++) {
		do_UserThreadJoin(pid, it->first);
	}
}

void CreateProcessusThreadsTable(int pid) {
	userThreads.P();
	userThreads[pid] = new SynchMap<int, UserThread*>();
	threadsCounters[pid] = new ThreadSafeCounter(0);
	userThreads.V();
}

void DestroyProcessusThreadsTable(int pid) {
	userThreads.SynchErase(pid);
	threadsCounters.SynchErase(pid);
}

#endif
