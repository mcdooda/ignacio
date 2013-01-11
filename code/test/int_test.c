#include "syscall.h"

int main() {
	int n;
	GetInt(&n);
	PutInt(n);
	PutChar('\n');
    return 0;
}