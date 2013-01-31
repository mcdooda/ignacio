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
	printf("Initialisation...\n");
	if (mem_init(NBINIT) == NULL) {
		printf("Erreur : tas plein OU plus de frame libres\n");
		return -1;
	}
	// ------------------------------------

	// ----------- tab 1 ------------------
	// allocation de NB1 entiers
	i = NB1;
	printf("Allocation de tab1 : %d entiers\n", &i);
	tab1 = (int*) mem_alloc(sizeof (int) * NB1);
	if (tab1 == NULL) {
		printf("Erreur : mémoire allouée pleine !\n");
		return 1;
	}

	printf("Écriture dans tab1...\n");
	for (i = 0; i < NB1; i++)
		tab1[i] = i;

	printf("Lecture de tab1 :\n");
	for (i = 0; i < NB1; i++)
		printf("tab1[%d] = %d [%s]\n", &i, &tab1[i], tab1[i]==i ? "OK" : "ERREUR");
	// ------------------------------------

	// ----------- tab 2 ------------------
	// allocation de NB2 caractères
	i = NB2;
	printf("Allocation de tab2 : %d caractères\n", &i);
	tab2 = (char*) mem_alloc(sizeof (char) * NB2);
	if (tab2 == NULL) {
		printf("Erreur : mémoire allouée pleine !\n");
		printf("Libération de tab1...\n");
		mem_free(tab1);
	}
	printf("Allocation de tab2 : %d caractères\n", &i);
	tab2 = (char*) mem_alloc(sizeof (char) * NB2);
	if (tab2 == NULL) {
		printf("Erreur : mémoire allouée pleine\n");
		return 2;
	}

	printf("Écriture dans tab2...\n");
	for (i = 0; i < NB2; i++)
		tab2[i] = 'A' + i;

	printf("Lecture de tab2 :\n");
	for (i = 0; i < NB2; i++)
		printf("tab2[%d] = %c [%s]\n", &i, &tab2[i], tab2[i]=='A'+i ? "OK" : "ERREUR");
	// ------------------------------------


	printf("FIN DU TEST\n");
	return 0;
}
