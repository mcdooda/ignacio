#ifdef CHANGED
#ifndef USERPROCESSUS_H
#define	USERPROCESSUS_H
#include <map>
#include "thread.h"
#include "synch.h"

class Processus {
public:
	
	Processus(char *name);
		
	Processus(Thread* th, int pid_, int ppid_);
	
	~Processus();
			
	Thread* getThread() {
		return t;
	}
	
	int getPid(){
		return pid;
	}
	
	void setPid(int p){
		pid = p;
	}
	
	int getPPid(){
		return ppid;
	}
	
	void setPPid(int p){
		ppid = p;
	}
	
	char * getName(){
		return nom;
	}
	
	std::map<int, Processus*> getMapFils(){
		return fils;	
	}
	
	void nbFilsPlus(){
		nb_fils ++;
	}
	
	void nbFilsMoins(){
		nb_fils --;
	}
	
	int getNbFils(){
		return nb_fils;
	}
				
	int AjouterFils(Processus *p);
	
	int RetirerFils(Processus *p);
	
	int WaitFils(int pid);
	
	int Wait();
	
private:
	Thread* t;
	char nom[50];
	int pid;
	int ppid;
	int nb_fils;
	std::map<int, Processus*> fils;
	Lock ajouterFils("mutex");
	
	int RetirerFils(int pid);
};

extern int do_ForkExec(char *filename);

#endif	/* USERPROCESSUS_H */
#endif // CHANGED
