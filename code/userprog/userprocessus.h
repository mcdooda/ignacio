#ifdef CHANGED
#ifndef USERPROCESSUS_H
#define	USERPROCESSUS_H
#include "thread.h"
#include "synch.h"
#include <map>
#include <list>


//
//class Processus {
//public:
//	
//	Processus(char *name);
//		
//	Processus(Thread* th, int pid_, int ppid_);
//	
//	~Processus();
//			
//	Thread* getThread() {
//		return t;
//	}
//	
//	int getPid(){
//		return pid;
//	}
//	
//	void setPid(int p){
//		pid = p;
//	}
//	
//	int getPPid(){
//		return ppid;
//	}
//	
//	void setPPid(int p){
//		ppid = p;
//	}
//	
//	char * getName(){
//		return nom;
//	}
//	
//	std::map<int, Processus*> getMapFils(){
//		return fils;	
//	}
//	
//	void nbFilsPlus(){
//		nb_fils ++;
//	}
//	
//	void nbFilsMoins(){
//		nb_fils --;
//	}
//	
//	int getNbFils(){
//		return nb_fils;
//	}
//				
//	int AjouterFils(Processus *p);
//	
//	int RetirerFils(Processus *p);
//	
//	int WaitFils(int id);
//
//	
//	int Wait();
//	
//	void Reveiller();
//	
//	bool FilsExiste(int id);
//
//	bool FilsEnVie(int id);
//	
//	void Exit();
//	
//private:
//	Thread* t;
//	char nom[50];
//	int pid;
//	int ppid;
//	int nb_fils;
//	std::map<int, Processus*> fils;
//	Lock addFils;
//	Lock mutmap;
//	Lock mutlist;
//	Lock *mutcond;
//	Condition cond;
//	std::list<int> filspid;
//};
void deleteProcessus(int pid);
extern int do_ForkExec(char *filename);
extern int GetPid(Thread* t);
extern void exitProc(int pid);
extern void addMainThread(Thread* m);
#endif	/* USERPROCESSUS_H */
#endif // CHANGED
