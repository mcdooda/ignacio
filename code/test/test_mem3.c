#include "syscall.h"
#include "libmem.h"
#include "libio.h"

#define TAILLE_BUFFER 128
#define TAILLE_MEMOIRE 4096

void aide() {
	printf("Aide :\n");
	printf("Saisir l'une des commandes suivantes\n");
	printf("\n");
	printf("a taille  :   allouer un bloc de la taille souhaitee\n");
	printf("l adresse :   librer un bloc alloue precedemment\n");
	printf("i         :   afficher la liste des emplacements memoire "
			"inocupes\n");
	printf("o         :   afficher la liste des emplacements memoire "
			"occupes\n");
	printf("M         :   afficher la liste de tous les emplacements "
			"memoire (libres et occupes)\n");
	printf("m         :   afficher le dump de la memoire\n");
	printf("h         :   afficher cette aide\n");
	printf("q         :   quitter ce programme\n");
	printf("\n");
}

void afficher_zone(void *adresse, int taille, int free) {
	int addr = (int) (adresse);
	printf("Zone %s, Adresse : %d, Taille : %d\n", free ? "libre" : "occupee", &addr, &taille);
}

void afficher_zone_libre(void *adresse, int taille, int free) {
	if (free)
		afficher_zone(adresse, taille, 1);
}

void afficher_zone_occupee(void *adresse, int taille, int free) {
	if (!free)
		afficher_zone(adresse, taille, 0);
}

int main() {
	char buffer[TAILLE_BUFFER];
	char commande, *adresse, *memoire;
	int offset, taille, i, temp, flush = 1;

	memoire = (char*) mem_init(TAILLE_MEMOIRE);
	if (memoire == NULL) {
		printf("Erreur : tas plein OU plus de frame libres\n");
	} else {
		aide();
		while (1) {
			printf("? ");
			commande = GetChar();
			switch (commande) {
				case 'a':
					scanf("%d", &taille);
					adresse = mem_alloc(taille);
					if (adresse == NULL) {
						printf("Echec de l'allocation\n");
					} else {
						temp = (int) (adresse - memoire);
						printf("Memoire allouee en %d\n", &temp);
					}
					flush = 0;
					break;
				case 'l':
					scanf("%d", &offset);
					mem_free(memoire + offset);
					printf("Memoire liberee\n");
					flush = 0;
					break;
				case 'i':
					mem_show(afficher_zone_libre);
					break;
				case 'o':
					mem_show(afficher_zone_occupee);
					break;
				case 'M':
					mem_show(afficher_zone);
					break;
				case 'm':
					for (i = 0; i < TAILLE_MEMOIRE; i++) {
						temp = memoire[i];
						printf("%d ", &temp);
					}
					printf("]\n");
					break;
				case 'h':
					aide();
					break;
				case 'q':
					return 0;
				default:
					printf("Commande inconnue !\n");
			}
			/* vide ce qu'il reste de la ligne dans le buffer d'entree */
			if (flush)
				GetString(buffer, TAILLE_BUFFER);
			flush = 1;
		}
	}
	return 0;
}
