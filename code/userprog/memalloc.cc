#ifdef CHANGED

#include "memalloc.h"

MemAlloc::MemAlloc(size_t size) {
	//TODO init
	tete_f = NULL;
//	tete_f = (fb*) mem;
//	tete_f->size = size;
//	tete_f->next = NULL;
	tete_d = NULL;

#if STRATEGIE == FIRST
	fit_strategy = &MemAlloc::mem_fit_first;
#elif STRATEGIE == BEST
	fit_strategy = &MemAlloc::mem_fit_best;
#else
	fit_strategy = &MemAlloc::mem_fit_worst;
#endif
}

MemAlloc::~MemAlloc() {
}

void* MemAlloc::mem_alloc(size_t size) {
	size += sizeof (db) + 1;
	if (size % 4 != 0)
		size += 4 - (size % 4);
	db* bloc = (db*) (this->*fit_strategy)(tete_f, size);
	if (bloc != NULL) {
		/*aucun bloc alloué*/
		if (tete_d == NULL) {
			tete_d = bloc;
			tete_d->next = NULL;
		} else {
			db* courant = tete_d;
			db* precedent = NULL;
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
		*(char*) ((char *) bloc + bloc->size - 1) = 0xFF;
		return (char *) bloc + sizeof (db);
	} else
		return NULL;
}

void MemAlloc::mem_free(void* bloc) {
	db* cour = tete_d;
	db* prec = NULL;

	bloc = (char *)bloc - sizeof (db);

	while (cour != NULL && (void*) cour < bloc) {
		prec = cour;
		cour = cour->next;
	}
	if ((void*) cour == bloc) {
		/*si bloc = tete*/
		if (prec == NULL) {
			tete_d = ((db*) bloc)->next;
		} else {
			prec->next = ((db*) bloc)->next;
		}

		/*linkage du bloc libéré*/
		/*si plus de mémoire libre*/
		if (tete_f == NULL) {
			tete_f = (fb*) bloc;
			((fb*) bloc)->next = NULL;
		} else {
			fb* cour2 = tete_f;
			fb* prec2 = NULL;
			while (cour2 != NULL && (void*) cour2 < bloc) {
				prec2 = cour2;
				cour2 = cour2->next;
			}
			/*si le bloc libéré va en tete*/
			if (prec2 == NULL) {
				((fb*) bloc)->next = tete_f;
				tete_f = (fb*) bloc;
			} else {
				((fb*) bloc)->next = cour2;
				prec2->next = (fb*) bloc;
			}

			/*fusion avec les blocs libres adjacents*/
			/*si pas en tete, fusion avec le précédent*/
			if (prec2 != NULL) {
				if ((char*) prec2 + prec2->size == bloc) {
					prec2->size += ((fb*) bloc)->size;
					prec2->next = ((fb*) bloc)->next;
					bloc = (void*) prec2;
				}
			}
			/*si pas en queue, fusion avec le suivant*/
			if (cour2 != NULL) {
				if ((char*)bloc + ((fb*) bloc)->size == (void*)cour2) {
					((fb*) bloc)->size += cour2->size;
					((fb*) bloc)->next = cour2->next;
				}
			}
		}
	}
}

// renvoie l'espace utilisateur disponible pour une adresse allouée avec malloc ou realloc.
size_t MemAlloc::mem_get_size(void * bloc) {
	return ((db*) bloc)->size - sizeof (db) - 1;
}

/* Affichage du contenu de l'allocateur */
void MemAlloc::mem_show(void (*print)(void *, size_t, int free)) {
	fb* f = tete_f;
	while (f != NULL) {
		(*print)((void*) f, f->size, 1);
		f = f->next;
	}

	db* d = tete_d;
	while (d != NULL) {
		(*print)((void*) d, d->size, 0);
		d = d->next;
	}
}

fb* MemAlloc::mem_fit_first(fb* tete, size_t size) {
	fb* courant = tete;
	fb* prec = NULL;
	/*tant que non nul et taille trop petite*/
	while (courant != NULL && courant->size < size) {
		prec = courant;
		courant = courant->next;
	}
	if (courant != NULL) {
		/*si taille parfaitement suffisante ou fragment généré de taille inférieur à l'en-tête*/
		if (courant->size == size || courant->size - size <= sizeof (db) + 1) {
			/*si non tete */
			if (prec != NULL) {
				prec->next = courant->next;
			} else {
				tete = courant->next;
			}
		}/*si résidu (bloc plus grand)*/
		else {
			fb * newFb = (fb*)((char*) courant + size);
			newFb->size = courant->size - size;
			newFb->next = courant->next;
			/*si non tete*/
			if (prec != NULL) {
				prec->next = newFb;
			} else {
				tete = newFb;
			}
			courant->size = size;
		}
		courant->next = NULL;
		tete_f = tete;
	}
	return courant;
}

fb* MemAlloc::mem_fit_best(fb* tete, size_t size) {
	fb* courant = tete;
	fb* prec = NULL;

	fb* courant_best = NULL;
	fb* prec_best = NULL;
	size_t residu_best;

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
		if (residu_best == 0 || residu_best <= sizeof (db) + 1) {
			/*si non tete */
			if (prec_best != NULL) {
				prec_best->next = courant_best->next;
			} else {
				tete = courant_best->next;
			}
		}/*si résidu (bloc plus grand)*/
		else {
			fb * newFb = (fb*)((char*) courant_best + size);
			newFb->size = courant_best->size - size;
			newFb->next = courant_best->next;
			/*si non tete*/
			if (prec_best != NULL) {
				prec_best->next = newFb;
			} else {
				tete = newFb;
			}
			courant_best->size = size;
		}
		courant_best->next = NULL;
		tete_f = tete;
	}
	return courant_best;
}

fb* MemAlloc::mem_fit_worst(fb* tete, size_t size) {
	fb* courant = tete;
	fb* prec = NULL;

	fb* courant_worst = NULL;
	fb* prec_worst = NULL;
	size_t residu_worst;

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
			}/*si résidu est plus grand que précédemment*/
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
		if (residu_worst == 0 || residu_worst <= sizeof (db) + 1) {
			/*si non tete*/
			if (prec_worst != NULL) {
				prec_worst->next = courant_worst->next;
			} else {
				tete = courant_worst->next;
			}
		}/*si résidu (bloc plus grand)*/
		else {
			fb * newFb = (fb*)((char*) courant_worst + size);
			newFb->size = courant_worst->size - size;
			newFb->next = courant_worst->next;
			/*si non tete*/
			if (prec_worst != NULL) {
				prec_worst->next = newFb;
			} else {
				tete = newFb;
			}
			courant_worst->size = size;
		}
		courant_worst->next = NULL;
		tete_f = tete;
	}
	return courant_worst;
}

#endif