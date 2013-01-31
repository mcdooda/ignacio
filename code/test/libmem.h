#ifndef LIBMEM_H
#define	LIBMEM_H
#define	NULL 0

#define FIRST 0
#define BEST 1
#define WORST 2

#define DEFAULT WORST

void* mem_init(int taille);

void* mem_reinit();

void* mem_alloc(int);

void mem_free(void*);

void mem_fit(int strategy);

/* renvoie l'espace utilisateur disponible pour une
   adresse allouée avec malloc ou realloc.
 */
int mem_get_size(void *);

/* Affichage du contenu de l'allocateur */
void mem_show(void (*print)(void *, int, int free));

#endif	/* LIBMEM_H */

