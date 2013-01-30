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
/* renvoie l'espace utilisateur disponible pour une
   adresse allouée avec malloc ou realloc.
   Requis pour la seconde partie du TD/TP (remplacement
   de malloc/realloc/free du system)
 */
int mem_get_size(void *);

/* Affichage du contenu de l'allocateur */
void mem_show(void (*print)(void *, int, int free));

struct fb;

/* Choix de la strategie et strategies usuelles */
typedef struct fb* (mem_fit_function_t) (struct fb *, int);

void mem_fit(int strategy);
struct fb* mem_fit_first(struct fb*, int);
struct fb* mem_fit_best(struct fb*, int);
struct fb* mem_fit_worst(struct fb*, int);

#endif	/* LIBMEM_H */

