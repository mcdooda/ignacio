#ifdef CHANGED

#include "userthread.h"
#include "machine.h"
#include "thread.h"
#include "system.h"

static void StartUserThread(int f) {
    machine->WriteRegister(PCReg,f);
    machine->WriteRegister(NextPCReg,f+4);
    machine->WriteRegister (StackReg, machine->ReadRegister(PCReg) + 2*PageSize);
    
    machine->Run ();
}

int do_UserThreadCreate(int f, int arg) {
    Thread *t = new Thread("forked thread user");
    t->Fork(StartUserThread, f);
    t->space = currentThread->space;
    currentThread->Yield();
    return 1;
}

void do_UserThreadExit(){
    currentThread->Finish();
}

#endif