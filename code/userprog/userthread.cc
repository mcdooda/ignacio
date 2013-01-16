#ifdef CHANGED

#include "userthread.h"
#include "machine.h"
#include "thread.h"
#include "system.h"
#include "synch.h"
#include <map>
#include <iostream>
//#include <stdlib>

static int NewThreadId();

class UserThread {
public:

	UserThread(int function, int argument, Thread* thread) :
	sem("UserThread", 0) {
		f = function;
		arg = argument;
		t = thread;
		id = NewThreadId();
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

	Thread* GetThread() {
		return t;
	}

	int GetStackBottom() {
		return stackBottom;
	}

	void SetStackBottom(int sb) {
		stackBottom = sb;
	}


private:
	int f;
	int arg;
	int id;
	int stackBottom;
	Semaphore sem;
	Thread* t;
};

static Semaphore semCreate("do_UserThreadCreate", 1);
static Semaphore semThreads("userThreads", 1);
static Semaphore semThreadId("NewThreadId", 1);
static Semaphore semFreeStack("GetFreeStackSpace", 1);
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
	UserThread* ut = userThreads[id];
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

	semFreeStack.P();
	freeStackPointer = currentThread->space->GetNextFreeStack();
	ut->SetStackBottom(freeStackPointer);
	semFreeStack.V();
	if (freeStackPointer != -1) {
		machine->WriteRegister(StackReg, freeStackPointer);
		machine->Run();
	} else {
		std::cout << "Creation du thread impossible" << std::endl;
		do_UserThreadExit();
	}
}

int do_UserThreadCreate(int f, int arg) {
	semCreate.P();
	Thread *t = new Thread("forked thread user");
	int id = SaveUserThread(new UserThread(f, arg, t));
	t->Fork(StartUserThread, id);
	t->space = currentThread->space;
	
	semCreate.V();
	return id;
}

void do_UserThreadExit() {
	UserThread* ut = GetUserThread(currentThread);
	ut->GetThread()->space->FreeStackSlot(ut->GetStackBottom());
	ut->GetSem()->V();
	currentThread->Finish();
}

void do_UserThreadJoin(int id) {
	UserThread* ut = GetUserThread(id);
	ut->GetSem()->P();
	int i;
	machine->ReadMem(ut->GetArg(), 4, &i);
	DeleteUserThread(ut);
}

void JoinUserThreads() {
	semThreads.P();
	for (std::map<int, UserThread*>::iterator it = userThreads.begin(); it != userThreads.end(); it++) {
		UserThread* ut = it->second;
		ut->GetThread()->Finish();
		DeleteUserThread(ut);
	}
	userThreads.clear();
	semThreads.V();
}

#endif