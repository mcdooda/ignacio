#ifndef LR
#define LR
#include "libsynch.h"
// ordre d'arrivée

typedef struct {
	sem_t R;
	sem_t mutex;
	int nb_lect;
} lecteur_redacteur_t;

typedef struct {
	lecteur_redacteur_t lecteur_redacteur;
	int iterations;
	int donnee;
	int id;
} donnees_thread_t;

void debut_lecture(lecteur_redacteur_t* lr);
void fin_lecture(lecteur_redacteur_t* lr);
void debut_redaction(lecteur_redacteur_t* lr);
void fin_redaction(lecteur_redacteur_t* lr);
void initialiser_lecteur_redacteur(lecteur_redacteur_t* lr);
void detruire_lecteur_redacteur(lecteur_redacteur_t* lr);

#endif