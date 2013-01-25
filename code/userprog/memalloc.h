#ifdef CHANGED
#ifndef MEMALLOC_H
#define	MEMALLOC_H

#include <stddef.h>

#define FIRST 0
#define BEST 1
#define WORST 2

#define STRATEGIE WORST

typedef struct fb {
	size_t size;
	struct fb* next;
} fb;

typedef struct db {
	size_t size;
	struct db* next;
} db;

class MemAlloc;

/* Choix de la strategie et strategies usuelles */
typedef fb* (MemAlloc::*mem_fit_function_t) (fb* tete, size_t size);

class MemAlloc {
public:
	MemAlloc(size_t size);
	~MemAlloc();

	void* mem_alloc(size_t);
	void mem_free(void* addr);
	// renvoie l'espace utilisateur disponible pour une adresse allouée avec malloc ou realloc.
	size_t mem_get_size(void *);
	/* Affichage du contenu de l'allocateur */
	void mem_show(void (*print)(void *, size_t, int free));

private:
	fb* tete_f;
	db* tete_d;
	mem_fit_function_t fit_strategy;

	fb* mem_fit_first(fb*, size_t);
	fb* mem_fit_best(fb*, size_t);
	fb* mem_fit_worst(fb*, size_t);
};

#endif	/* MEMALLOC_H */
#endif

