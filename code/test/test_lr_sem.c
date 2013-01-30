#include "test_lr_sem.h"
#include "libio.h"
#include "syscall.h"
// ordre d'arrivée

void long_loop() {
	unsigned int fin = 1000,i=0;
	while (i < fin) {
		i++;
	}
}

void debut_lecture(lecteur_redacteur_t* lr) {
	sem_P(lr->mutex);
	lr->nb_lect++;
	if (lr->nb_lect == 1)
		sem_P(lr->R);
	sem_V(lr->mutex);
}

void fin_lecture(lecteur_redacteur_t* lr) {
	sem_P(lr->mutex);
	lr->nb_lect--;
	if (lr->nb_lect == 0)
		sem_V(lr->R);
	sem_V(lr->mutex);
}

void debut_redaction(lecteur_redacteur_t* lr) {
	sem_P(lr->R);
}

void fin_redaction(lecteur_redacteur_t* lr) {
	sem_V(lr->R);
}

void initialiser_lecteur_redacteur(lecteur_redacteur_t* lr) {
	lr->R = sem_create("sem_red", 1);
	lr->mutex = sem_create("sem_all", 1);
	lr->nb_lect = 0;
}

void detruire_lecteur_redacteur(lecteur_redacteur_t* lr) {
	sem_destroy(lr->R);
	sem_destroy(lr->mutex);
}

void lecteur(void *args) {
	donnees_thread_t *d = (donnees_thread_t *) args;
	int i, valeur;

	for (i = 0; i < d->iterations; i++) {
		long_loop();
		debut_lecture(&d->lecteur_redacteur);

		printf("%s","debut lecture ");
		valeur = d->donnee;
		long_loop();
		if (valeur != d->donnee) {
			//lecture incohérente
			printf("%s","X");
		} else {
			printf("%s","lecture coherente ");
		}

		fin_lecture(&d->lecteur_redacteur);
	}
	UserThreadExit();
}

void redacteur(void *args) {
	donnees_thread_t *d = (donnees_thread_t *) args;
	int i, valeur;
	for (i = 0; i < d->iterations; i++) {
		debut_redaction(&d->lecteur_redacteur);

		printf("%s","debut redaction........ ");
		valeur = d->id;
		d->donnee = valeur;
		long_loop();
		if (valeur != d->donnee) {
			//Rédaction incohérente
			printf("%s","X");
		} else {
			printf("%s","redaction coherente...... ");
		}

		fin_redaction(&d->lecteur_redacteur);
	}
	UserThreadExit();
}

int main(int argc, char *argv[]) {
	int threads[6];
	donnees_thread_t donnees_thread;
	int i, nb_lecteurs, nb_redacteurs;

	nb_lecteurs = 3;
	nb_redacteurs = 3;
	donnees_thread.iterations = 3;

	initialiser_lecteur_redacteur(&donnees_thread.lecteur_redacteur);

	for (i = 0; i < nb_lecteurs + nb_redacteurs; i++) {
		donnees_thread.id = i;
		if (i % 2) {
			threads[i] = UserThreadCreate(lecteur, (void*) &donnees_thread);
		} else {
			threads[i] = UserThreadCreate(redacteur, (void*) &donnees_thread);
		}
	}

	for (i = 0; i < (nb_lecteurs + nb_redacteurs); i++)
		UserThreadJoin(threads[i]);

	detruire_lecteur_redacteur(&donnees_thread.lecteur_redacteur);

	return 0;
}



