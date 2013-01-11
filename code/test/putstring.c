#include "syscall.h"

int main() {
	PutString("str court\n");
	PutString("str qui fait 19car\n"); // MAX_STRING_SIZE - 1 
	PutString("Chaine plus longue que la taille maximale autorisée");
	PutChar('\n');
	return 0;
}


