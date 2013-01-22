#ifdef CHANGED
#ifndef USERPROCESSUS_H
#define	USERPROCESSUS_H
#include <map>
#include <list>
#include "thread.h"
#include "synch.h"

class Processus {
public:
	
	Processus(char *name);
		
	Processus(char *name, Thread* th, int pid_, int ppid_);
	
	~Processus();
			
	Thread* getThread() {
		return t;
	}
	
	int getPid(){
		return pid;
	}
	
	int getPPid(){
		return ppid;
	}
	
	char * getName(){
		return nom;
	}
				
	int AjouterFils(Processus *p);
	
	int WaitFils(int id);
	
	int Wait();
	
	bool FilsExiste(int id);

	bool FilsEnVie(int id);
	
private:
	Thread* t;
	char nom[50];
	int pid;
	int ppid;
	std::map<int, Processus> fils;
	Lock mutmap;
	Lock mutlist;
	Lock *mutcond;
	Condition cond;
	std::list<int> filspid;
	int RetirerFils(int pid);
};

extern int do_ForkExec(char *filename);

#endif	/* USERPROCESSUS_H */
#endif // CHANGED
