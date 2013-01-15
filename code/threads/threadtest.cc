// threadtest.cc 
//      Simple test case for the threads assignment.
//
//      Create two threads, and have them context switch
//      back and forth between themselves by calling Thread::Yield, 
//      to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"

#ifdef CHANGED
#include "synch.h"
Lock *mutex = new Lock("mutex");
Condition *cons = new Condition("cons");
Condition *prod = new Condition("prod");
int max = 5;
int taille = 0;
#endif // CHANGED

//----------------------------------------------------------------------
// SimpleThread
//      Loop 5 times, yielding the CPU to another ready thread 
//      each iteration.
//
//      "which" is simply a number identifying the thread, for debugging
//      purposes.
//----------------------------------------------------------------------

void
SimpleThread (int which)
{
	int num;

    for (num = 0; num < 5; num++)
      {
	  printf ("*** thread %d looped %d times\n", which, num);
	  currentThread->Yield ();
      }
}
#ifdef CHANGED
void MutexTest(int which){
	mutex->Acquire();
	for(int i=0; i<10; i++){
		printf("*** thread %d \n", which);
	    currentThread->Yield ();
	}
	mutex->Release();
}

void Producteur(int which){
	for(int i=0; i<10; i++){
//	while(true){
		mutex->Acquire();
		while(taille != 0){
			printf("*** producteur %d en attente\n", which);
			prod->Wait(mutex);
			printf("*** producteur %d libéré\n", which);
		}
		taille+=max;
		printf("producteur %d taille %d\n", which, taille);
		cons->Signal(mutex);
		mutex->Release();
		currentThread->Yield ();
	}
}

void Consommateur(int which){
	for(int i=0; i<10; i++){
//	while(true){
		mutex->Acquire();
		while(taille == 0){
			printf("*** consommateur %d en attente\n", which);
			cons->Wait(mutex);
			printf("*** consommateur %d libéré\n", which);
		}
		taille--;
		printf("consommateur %d taille %d\n", which, taille);
		prod->Signal(mutex);
		mutex->Release();
		currentThread->Yield ();
	}
}
#endif // CHANGED
//----------------------------------------------------------------------
// ThreadTest
//      Set up a ping-pong between two threads, by forking a thread 
//      to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void
ThreadTest ()
{
    DEBUG ('t', "Entering SimpleTest\n");
#ifdef CHANGED
	Thread *t1 = new Thread("t1");
	Thread *t2 = new Thread("t2");
	Thread *t3 = new Thread("t3");
	Thread *t4 = new Thread("t4");
	Thread *t5 = new Thread("t5");
	Thread *t6 = new Thread("t6");
	t1->Fork(MutexTest, 1);
	t2->Fork(MutexTest, 2);
	t3->Fork(MutexTest, 3);
	t1 = new Thread("t1");
	t2 = new Thread("t2");
	t3 = new Thread("t3");
	t4->Fork(Consommateur, 4);
	t2->Fork(Consommateur, 1);
	t5->Fork(Producteur, 6);
	t6->Fork(Consommateur, 5);
	t3->Fork(Producteur, 3);
	t1->Fork(Producteur, 2);
#else
    Thread *t = new Thread ("forked thread");
    t->Fork (SimpleThread, 1);
	
	Thread *t2 = new Thread ("forked thread 2");
	t2->Fork (SimpleThread, 2);
	
    SimpleThread (0);
#endif //CHANGED
}
