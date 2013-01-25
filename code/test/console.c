#include "syscall.h"
#include "libio.h"
#include "libstring.h"
#include "libchar.h"

#define MAX_LINE_SIZE 200
#define MAX_ARG_COUNT 10

int readarg(const char* line, char args[MAX_ARG_COUNT][MAX_LINE_SIZE]) {
	int i, j, k = 0;
	char c;
	for (i = 0; line[i] != '\0'; i++) {
		c = line[i];
		while (isblank(c)) {
			i++;
			c = line[i];
		}
		for (j = i; isprint(line[j]); j++);
		if (j > i) {
			stringcopy(line + i, args[k], j - i);
			k++;
		}
		i = j;
	}
	return k;
}

int main(int argc, char* argv[]) {
	char line[MAX_LINE_SIZE];
	char args[MAX_ARG_COUNT][MAX_LINE_SIZE];

	while (1) {
		printf("NachOS$ ");
/*
		PutString("NachOS$ ");
*/
		GetString(line, MAX_LINE_SIZE);
		readarg(line, args);
		if (stringeq(args[0], "exit")) {
			break;
		} else {
			ForkExec(args[0]);
		}
	}

	return 0;
}
