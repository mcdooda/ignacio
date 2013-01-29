#include "syscall.h"
#include "libmem.h"
#include "libio.h"

#define NBINIT (sizeof (int) * 15)
#define NB1 10
#define NB2 10

int main() {
	int *tab1, i;
	char *tab2;

	// --------- initialisation -----------
	if (mem_init(NBINIT) == NULL) {
		printf("Erreur : tas plein OU plus de frame libres\n");
		return -1;
	}
	// ------------------------------------

	// ----------- tab 1 ------------------
	// allocation de NB1 entiers
	tab1 = (int*) mem_alloc(sizeof (int) * NB1);
	if (tab1 == NULL) {
		printf("Erreur : mémoire allouée pleine\n");
		return 1;
	}

	printf("ECRITURE TAB1\n");
	for (i = 0; i < NB1; i++)
		tab1[i] = i;

	printf("LECTURE TAB1\n");
	for (i = 0; i < NB1; i++)
		printf("tab1[%d] = %d\n", &i, &tab1[i]);
	// ------------------------------------

	// ----------- tab 2 ------------------
	// allocation de NB2 caractères
	tab2 = (char*) mem_alloc(sizeof (char) * NB2);
	if (tab2 == NULL) {
		printf("Erreur : mémoire allouée pleine\n");
		printf("Libération de tab1\n");
		mem_free(tab1);
	}
	tab2 = (char*) mem_alloc(sizeof (char) * NB2);
	if (tab2 == NULL) {
		printf("Erreur : mémoire allouée pleine\n");
		return 2;
	}

	printf("ECRITURE TAB2\n");
	for (i = 0; i < NB2; i++)
		tab2[i] = 'A' + i;

	printf("LECTURE TAB2\n");
	for (i = 0; i < NB2; i++)
		printf("tab2[%d] = %c\n", &i, &tab2[i]);
	// ------------------------------------


	printf("FIN\n");
	return 0;
}
