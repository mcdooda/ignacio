#include "libmem.h"
#include "syscall.h"
#include "libio.h"

#define FIRST 0
#define BEST 1
#define WORST 2

#define STRATEGIE FIRST

struct fb {
	int size;
	struct fb* next;
};

struct db {
	int size;
	struct db* next;
};

struct fb* tete_f;
struct db* tete_d;

mem_fit_function_t* fit_strategy;

void* mem_init(int taille) {
	tete_f = (struct fb*) Sbrk(taille);
	printf("ntm\n");
	if(tete_f == NULL)
		return NULL;
	
	tete_f->size = taille;
	tete_f->next = NULL;
	tete_d = NULL;

#if STRATEGIE == FIRST
	mem_fit(mem_fit_first);
#elif STRATEGIE == BEST
	mem_fit(mem_fit_best);
#else
	mem_fit(mem_fit_worst);
#endif

	return tete_f;
}

void* mem_alloc(int size) {
	struct db* bloc;

	size += sizeof (struct db) + 1;
	if (size % 4 != 0)
		size += 4 - (size % 4);
	bloc = (struct db*) fit_strategy(tete_f, size);
	if (bloc != NULL) {
		/*aucun bloc alloué*/
		if (tete_d == NULL) {
			tete_d = bloc;
			tete_d->next = NULL;
		} else {
			struct db* courant = tete_d;
			struct db* precedent = NULL;
			while (courant != NULL && courant < bloc) {
				precedent = courant;
				courant = courant->next;
			}
			if (courant == NULL) {
				precedent->next = bloc;
			} else {
				if (precedent == NULL) {
					bloc->next = courant;
					tete_d = bloc;
				} else {
					precedent->next = bloc;
					bloc->next = courant;
				}
			}
		}
		*(char*) ((void *) bloc + bloc->size - 1) = 0xFF;
		return (void *) bloc + sizeof (struct db);
	} else
		return NULL;
}

void mem_free(void* bloc) {
	struct db* cour = tete_d;
	struct db* prec = NULL;

	bloc -= sizeof (struct db);

	while (cour != NULL && (void*) cour < bloc) {
		prec = cour;
		cour = cour->next;
	}
	if ((void*) cour == bloc) {
		/*si bloc = tete*/
		if (prec == NULL) {
			tete_d = ((struct db*) bloc)->next;
		} else {
			prec->next = ((struct db*) bloc)->next;
		}

		/*linkage du bloc libéré*/
		/*si plus de mémoire libre*/
		if (tete_f == NULL) {
			tete_f = (struct fb*) bloc;
			((struct fb*) bloc)->next = NULL;
		} else {
			struct fb* cour2 = tete_f;
			struct fb* prec2 = NULL;
			while (cour2 != NULL && (void*) cour2 < bloc) {
				prec2 = cour2;
				cour2 = cour2->next;
			}
			/*si le bloc libéré va en tete*/
			if (prec2 == NULL) {
				((struct fb*) bloc)->next = tete_f;
				tete_f = (struct fb*) bloc;
			} else {
				((struct fb*) bloc)->next = cour2;
				prec2->next = (struct fb*) bloc;
			}

			/*fusion avec les blocs libres adjacents*/
			/*si pas en tete, fusion avec le précédent*/
			if (prec2 != NULL) {
				if ((void*) prec2 + prec2->size == bloc) {
					prec2->size += ((struct fb*) bloc)->size;
					prec2->next = ((struct fb*) bloc)->next;
					bloc = (void*) prec2;
				}
			}
			/*si pas en queue, fusion avec le suivant*/
			if (cour2 != NULL) {
				if (bloc + ((struct fb*) bloc)->size == cour2) {
					((struct fb*) bloc)->size += cour2->size;
					((struct fb*) bloc)->next = cour2->next;
				}
			}
		}
	}
}

/* renvoie l'espace utilisateur disponible pour une
   adresse allouée avec malloc ou realloc.
   Requis pour la seconde partie du TD/TP (remplacement
   de malloc/realloc/free du system)
 */
int mem_get_size(void * bloc) {
	return ((struct db*) bloc)->size - sizeof (struct db) - 1;
}

/* Affichage du contenu de l'allocateur */
void mem_show(void (*print)(void *, int, int free)) {
	struct db* d;
	struct fb* f = tete_f;

	while (f != NULL) {
		print((void*) f, f->size, 1);
		f = f->next;
	}

	d = tete_d;
	while (d != NULL) {
		print((void*) d, d->size, 0);
		d = d->next;
	}
}

/* Facultatif */

void mem_fit(mem_fit_function_t* fit) {
	fit_strategy = fit;
}

struct fb* mem_fit_first(struct fb* tete, int size) {
	struct fb* courant = tete;
	struct fb* prec = NULL;
	/*tant que non nul et taille trop petite*/
	while (courant != NULL && courant->size < size) {
		prec = courant;
		courant = courant->next;
	}
	if (courant != NULL) {
		/*si taille parfaitement suffisante ou fragment généré de taille inférieur à l'en-tête*/
		if (courant->size == size || courant->size - size <= sizeof (struct db) + 1) {
			/*si non tete */
			if (prec != NULL) {
				prec->next = courant->next;
			} else {
				tete = courant->next;
			}
		}			/*si résidu (bloc plus grand)*/
		else {
			struct fb* new = (void*) courant + size;
			new->size = courant->size - size;
			new->next = courant->next;
			/*si non tete*/
			if (prec != NULL) {
				prec->next = new;
			} else {
				tete = new;
			}
			courant->size = size;
		}
		courant->next = NULL;
		tete_f = tete;
	}
	return courant;
}

struct fb* mem_fit_best(struct fb* tete, int size) {
	struct fb* courant = tete;
	struct fb* prec = NULL;

	struct fb* courant_best = NULL;
	struct fb* prec_best = NULL;
	int residu_best;

	/*tant que non nul*/
	while (courant != NULL) {
		/*si taille suffisante ET résidu est plus petit que précédemment*/
		if (courant->size >= size && (courant_best == NULL || courant->size - size < residu_best)) {
			courant_best = courant;
			prec_best = prec;
			residu_best = courant->size - size;
			/*si taille parfaite*/
			if (residu_best == 0) {
				break;
			}
		}
		prec = courant;
		courant = courant->next;
	}

	if (courant_best != NULL) {
		/*si taille parfaitement suffisante ou fragment généré de taille inférieur à l'en-tête*/
		if (residu_best == 0 || residu_best <= sizeof (struct db) + 1) {
			/*si non tete */
			if (prec_best != NULL) {
				prec_best->next = courant_best->next;
			} else {
				tete = courant_best->next;
			}
		}			/*si résidu (bloc plus grand)*/
		else {
			struct fb* new = (void*) courant_best + size;
			new->size = courant_best->size - size;
			new->next = courant_best->next;
			/*si non tete*/
			if (prec_best != NULL) {
				prec_best->next = new;
			} else {
				tete = new;
			}
			courant_best->size = size;
		}
		courant_best->next = NULL;
		tete_f = tete;
	}
	return courant_best;
}

struct fb* mem_fit_worst(struct fb* tete, int size) {
	struct fb* courant = tete;
	struct fb* prec = NULL;

	struct fb* courant_worst = NULL;
	struct fb* prec_worst = NULL;
	int residu_worst;

	/*tant que non nul*/
	while (courant != NULL) {
		/*si taille suffisante*/
		if (courant->size >= size) {
			/*si taille parfaite*/
			if (courant->size - size == 0) {
				courant_worst = courant;
				prec_worst = prec;
				residu_worst = 0;
				break;
			}				/*si résidu est plus grand que précédemment*/
			else if (courant_worst == NULL || courant->size - size > residu_worst) {
				courant_worst = courant;
				prec_worst = prec;
				residu_worst = courant->size - size;
			}
		}
		prec = courant;
		courant = courant->next;
	}

	if (courant_worst != NULL) {
		/*si taille parfaitement suffisante ou fragment généré de taille inférieur à l'en-tête*/
		if (residu_worst == 0 || residu_worst <= sizeof (struct db) + 1) {
			/*si non tete*/
			if (prec_worst != NULL) {
				prec_worst->next = courant_worst->next;
			} else {
				tete = courant_worst->next;
			}
		}			/*si résidu (bloc plus grand)*/
		else {
			struct fb* new = (void*) courant_worst + size;
			new->size = courant_worst->size - size;
			new->next = courant_worst->next;
			/*si non tete*/
			if (prec_worst != NULL) {
				prec_worst->next = new;
			} else {
				tete = new;
			}
			courant_worst->size = size;
		}
		courant_worst->next = NULL;
		tete_f = tete;
	}
	return courant_worst;
}

