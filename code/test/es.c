#include "syscall.h"
#include "libio.h"

int main(int argc, char* argv[]) {
	int i, j, k;
	char str[20];
	char c;
	int t[2];
	t[0] = 1;
	t[1] = 2;
	printf("%d %p \n",&t[0],t);
	printf("Entrez 3 nombres :\n");
	scanf("%d %d %d", &i, &j, &k);
	printf("Les nombres sont : %d %d %d %%\n", &i, &j, &k);
	printf("Entrez une chaine :\n");
	scanf("%s", str);
	printf("La chaine est : %s\n", str);
	printf("Entrez un caractere :\n");
	scanf("%c", &c);
	printf("Le caractere est : %c\n", &c);

	return 0;
}

