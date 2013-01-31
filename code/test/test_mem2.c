#include "syscall.h"
#include "libmem.h"
#include "libio.h"

#define TAILLE_BUFFER 128
#define TAILLE_MEMOIRE 4096

#define MAX_ALLOC 1024
static int nb_free, nb_busy;
static void *allocs[MAX_ALLOC];
static int sizes[MAX_ALLOC];
static int free_size[MAX_ALLOC];
static int busy_size[MAX_ALLOC];
static int position;

void decompte_zone(void *adresse, int taille, int free) {
	if (free)
		free_size[nb_free++] = taille;
	else
		busy_size[nb_busy++] = taille;
}

void afficher_zone(void *adresse, int taille, int free) {
	int addr;
	decompte_zone(adresse, taille, free);
	addr = (int) adresse;
	printf("Zone %s, Adresse : %d, Taille : %d\n", free ? "libre" : "occupee", &addr, &taille);
}

void my_mem_init(int s) {
	position = 0;
	if (s > 0) {
		if (mem_init(s) == NULL) {
			printf("Erreur : tas plein OU plus de frame libres\n");
			Exit(-1);
		}
	} else
		mem_reinit();
}

void *my_mem_alloc(int s) {
	void *result = mem_alloc(s);
	int res = (int) result;
	sizes[position] = s;
	allocs[position++] = result;
	printf("Allocation de %d octets en %d\n", &s, &res);
	return result;
}

void my_mem_free(int num) {
	int addr = (int) allocs[num];
	printf("Liberation de la zone en %d\n", &addr);
	mem_free(allocs[num]);
}

void my_mem_show(void (*print)(void *, int, int free)) {
	nb_free = 0;
	nb_busy = 0;
	mem_show(print);
}

void assert(int b) {
	if (!b) {
		printf("TEST ÉCHOUÉ\n\n");
		Exit(-1);
	}
}

int main() {
	int struct_fb_size, struct_bb_size;
	int i, j, nb_alloc = 0, size, addr;
	char *result;

	my_mem_init(TAILLE_MEMOIRE);
	printf("Test de l'etat initial de la memoire :\n");
	my_mem_show(afficher_zone);
	assert(nb_free == 1);
	printf("TEST OK\n\n");

	printf("Test de base, serie d'allocations :\n");
	my_mem_alloc(8);
	my_mem_alloc(16);
	my_mem_alloc(4);
	my_mem_alloc(12);
	printf("Etat de la memoire :\n");
	my_mem_show(afficher_zone);
	assert(nb_free == 1 && nb_busy == 4);
	printf("TEST OK\n\n");

	printf("Test de mem_init :\n");
	my_mem_init(-1);
	printf("Etat de la memoire :\n");
	my_mem_show(afficher_zone);
	assert(nb_free == 1);
	printf("TEST OK\n\n");

	printf("Test d'allocation puis liberation :\n");
	struct_bb_size = free_size[0];
	my_mem_alloc(16);
	my_mem_show(decompte_zone);
	struct_bb_size -= free_size[0] + 16;
	my_mem_alloc(4);
	my_mem_alloc(30);
	my_mem_alloc(1);
	my_mem_alloc(64);
	my_mem_alloc(24);
	my_mem_alloc(23);
	my_mem_free(2);
	printf("Etat de la memoire :\n");
	my_mem_show(afficher_zone);
	assert(nb_free == 2 && nb_busy == 6);
	printf("TEST OK\n\n");

	printf("Verification de la taille des allocations (multiples de 4)\n");
	for (i = 0; i < nb_busy; i++)
		assert((busy_size[i] & 3) == 0);
	printf("TEST OK\n\n");

	printf("Test de fusion amont :\n");
	my_mem_free(1);
	printf("Etat de la memoire :\n");
	my_mem_show(afficher_zone);
	struct_fb_size = free_size[0] - 36;
	assert(nb_free == 2 && nb_busy == 5);
	printf("TEST OK\n\n");

	printf("Test de fusion aval :\n");
	my_mem_free(3);
	printf("Etat de la memoire :\n");
	my_mem_show(afficher_zone);
	assert(nb_free == 2 && nb_busy == 4);
	printf("TEST OK\n\n");

	printf("Test de fusion amont+aval :\n");
	my_mem_free(5);
	my_mem_show(decompte_zone);
	assert(nb_free == 3 && nb_busy == 3);
	my_mem_free(4);
	printf("Etat de la memoire :\n");
	my_mem_show(afficher_zone);
	assert(nb_free == 2 && nb_busy == 2);
	printf("TEST OK\n\n");

	addr = (int) struct_fb_size;
	printf("Apparemment la taille de votre entete de bloc libre est de %d "
			"octets\n", &addr);
	addr = (int) struct_bb_size;
	printf("Apparemment la taille de votre entete de bloc occupe est de %d "
			"octets\n", &addr);
	printf("\n");

	printf("Test d'allocation/liberation de tout l'espace :\n");
	my_mem_init(-1);
	my_mem_alloc(TAILLE_MEMOIRE - struct_bb_size);
	printf("Etat de la memoire :\n");
	my_mem_show(afficher_zone);
	assert(nb_free == 0 && nb_busy == 1);
	my_mem_free(0);
	printf("Etat de la memoire :\n");
	my_mem_show(afficher_zone);
	assert(nb_free == 1 && nb_busy == 0);
	printf("TEST OK\n\n");

	printf("Test d'allocation/libération avec utilisation de la mémoire :\n");
	my_mem_init(-1);
	my_mem_alloc(16);
	my_mem_alloc(4);
	my_mem_alloc(30);
	my_mem_alloc(1);
	my_mem_alloc(64);
	my_mem_alloc(24);
	my_mem_alloc(23);
	for (i = 0; i < position; i++)
		for (j = 0; j < sizes[i]; j++)
			((char *) allocs[i])[j] = (char) Random();
	my_mem_free(1);
	my_mem_free(3);
	my_mem_free(5);
	my_mem_alloc(4);
	for (i = 0; i < position; i++)
		if ((i > 5) || (i & 1) == 0)
			for (j = 0; j < sizes[i]; j++)
				((char *) allocs[i])[j] = (char) Random();
	printf("Etat de la memoire :\n");
	my_mem_show(afficher_zone);
	assert(nb_free == 3 && nb_busy == 5);
	printf("TEST OK\n\n");

	printf("Test final, serie aléatoire d'allocations/libérations aléatoires "
			"avec utilisation de la mémoire :\n");
	my_mem_init(-1);
	for (i = 0; i < 1000; i++) {
		if (nb_alloc && (Random() & 1)) {
			for (j = 0; !allocs[j]; j++) {
			}
			my_mem_free(j);
			allocs[j] = NULL;
			nb_alloc--;
		} else {
			size = Random() & 511;
			result = my_mem_alloc(size);
			if (result) {
				for (j = 0; j < size; j++)
					result[j] = (char) Random();
				nb_alloc++;
			}
		}
	}
	printf("TEST OK\n\n");

	return 0;
}
