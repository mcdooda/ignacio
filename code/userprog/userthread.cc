#ifdef CHANGED

#include "userthread.h"
#include "machine.h"
#include "thread.h"
#include "system.h"
#include "synch.h"
#include "threadsafecounter.h"
#include <map>
#include <iostream>
//#include <stdlib>

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
static Semaphore semThreads("userThreads", 1);
static Semaphore semThreadId("NewThreadId", 1);
static Semaphore semFreeStack("GetFreeStackSpace", 1);
static Semaphore semJoin("UserThreadJoin", 1);
static ThreadSafeCounter threadCounter(0);

static int nextTid = 1;
static std::map<int, UserThread*> userThreads;

static int NewThreadId() {
	semThreadId.P();
	int tid = nextTid;
	nextTid++;
	semThreadId.V();
	return tid;
}

static int SaveUserThread(UserThread* ut) {
	semThreads.P();
	userThreads[ut->GetId()] = ut;
	semThreads.V();
	return ut->GetId();
}

static void DeleteUserThread(UserThread* ut) {
	semThreads.P();
	std::map<int, UserThread*>::iterator it = userThreads.find(ut->GetId());
	userThreads.erase(it);
	delete ut;
	semThreads.V();
}

static UserThread* GetUserThread(Thread* thread) {
	semThreads.P();
	UserThread* ut = NULL;
	for (std::map<int, UserThread*>::iterator it = userThreads.begin(); it != userThreads.end(); it++) {
		if (it->second->GetThread() == thread) {
			ut = it->second;
			break;
		}
	}
	semThreads.V();
	return ut;
}

static UserThread* GetUserThread(int id) {
	semThreads.P();
	std::map<int, UserThread*>::iterator it = userThreads.find(id);
	UserThread* ut = NULL;
	if (it != userThreads.end()) {
		ut = it->second;
	}
	semThreads.V();
	return ut;
}

static void StartUserThread(int id) {
	int freeStackPointer;
	UserThread* ut = GetUserThread(id);
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
		do_UserThreadExit();
	}
}

int do_UserThreadCreate(int f, int arg, int pE) {
	semCreate.P(); //TODO: verifier la safe-threadry
	Thread *t = new Thread("forked thread user");
	UserThread* ut = new UserThread(f, arg, pE, t);
	int id = SaveUserThread(ut);
	t->Fork(StartUserThread, id);
	ut->GetSemStart()->P();
	if (ut->Failed()) {
		id = -1;
		DeleteUserThread(ut);
	} else {
		t->space = currentThread->space;
		++threadCounter;
	}
	semCreate.V();
	return id;
}

void do_UserThreadExit() {
	UserThread* ut = GetUserThread(currentThread);
	ut->GetThread()->space->FreeStackSlot(ut->GetStackBottom());
	ut->GetSem()->V();
	--threadCounter;
	currentThread->Finish();
}

void do_UserThreadJoin(int id) {
	UserThread* ut = GetUserThread(id); // thread safe!
	// pas thread safe!
	if (ut != NULL) {
		ut->GetSem()->P();
		int i;
		machine->ReadMem(ut->GetArg(), 4, &i);
		DeleteUserThread(ut);
	}
}

void JoinUserThreads() {
	threadCounter.waitUntilValue(0);
	std::map<int, UserThread*> threads = userThreads;
	for (std::map<int, UserThread*>::iterator it = threads.begin(); it != threads.end(); it++) {
		do_UserThreadJoin(it->first);
	}
}

#endif