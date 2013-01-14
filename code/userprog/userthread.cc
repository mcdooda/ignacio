#ifdef CHANGED

#include "userthread.h"
#include "machine.h"
#include "thread.h"
#include "system.h"
#include "synch.h"
#include <map>

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

private:
    int f;
    int arg;
    int id;
    Semaphore sem;
    Thread* t;
};

std::map<int, UserThread*> userThreads;

static int NewThreadId() {
    int id = 1;
    while (userThreads.find(id) != userThreads.end()) {
        id++;
    }
    return id;
}

static int SaveUserThread(UserThread* ut) {
    userThreads[ut->GetId()] = ut;
    return ut->GetId();
}

static void DeleteUserThread(int id) {
    std::map<int, UserThread*>::iterator it = userThreads.find(id);
    delete it->second;
    userThreads.erase(it);
}

static UserThread* GetUserThread(Thread* thread) {
    UserThread* ut = NULL;
    for (std::map<int, UserThread*>::iterator it = userThreads.begin(); it != userThreads.end(); it++) {
        if (it->second->GetThread() == thread) {
            ut = it->second;
            break;
        }
    }
    return ut;
}

static void StartUserThread(int id) {
    UserThread* ut = userThreads[id];
    machine->WriteRegister(4, ut->GetArg());
    machine->WriteRegister(PCReg, ut->GetF());
    machine->WriteRegister(NextPCReg, ut->GetF() + 4);
    
    //machine->WriteRegister(StackReg, machine->ReadRegister(PCReg) + 4 * PageSize);
    
    machine->WriteRegister(StackReg, currentThread->space->GetNextFreeStack());
    machine->Run();
}

int do_UserThreadCreate(int f, int arg) {
    Thread *t = new Thread("forked thread user");
    int id = SaveUserThread(new UserThread(f, arg, t));
    t->Fork(StartUserThread, id);
    t->space = currentThread->space;
    return id;
}

void do_UserThreadExit() {
    UserThread* ut = GetUserThread(currentThread);
    ut->GetSem()->V();
    currentThread->Finish();
}

void do_UserThreadJoin(int id) {
    UserThread* ut = userThreads[id];
    ut->GetSem()->P();
    int i;
    machine->ReadMem(ut->GetArg(), 4, &i);
    DeleteUserThread(ut->GetId());
}

#endif