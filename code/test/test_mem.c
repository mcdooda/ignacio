#include "syscall.h"
#include "libmem.h"
#include "libio.h"

int main() {
	int *tab, i;
	printf("coucou\n");

	mem_init(sizeof (int) * 10);
	printf("recoucou\n");
	tab = (int*) mem_alloc(sizeof (int) * 10);
	printf("rerecoucou\n");

	for (i = 0; i < 10; i++)
		tab[i] = i;

	for (i = 0; i < 10; i++)
		printf("tab[%d] = %d", &i, &tab[i]);

	return 0;
}
