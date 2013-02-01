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

void invalidArguments(char args[MAX_ARG_COUNT][MAX_LINE_SIZE], int argc, int required) {
	int i;
	printf("Invalid arguments for %s: ", args[0]);
	if (argc == 1) {
		printf("<none> ");
	} else {
		for (i = 1; i < argc; i++) {
			printf("%s ", args[i]);
		}
	}
	if (required > 1) {
		printf("(%d arguments required)\n", &required);
	} else {
		printf("(%d argument required)\n", &required);
	}
}

int main(int argc, char* argv[]) {
	char line[MAX_LINE_SIZE];
	char wd[MAX_LINE_SIZE];
	char args[MAX_ARG_COUNT][MAX_LINE_SIZE];
	int pid;

	ChDir("..");

	while (1) {
		Pwd(wd);
		printf("NachOS-%s$ ", wd);
		GetString(line, MAX_LINE_SIZE);
		if (stringlen(line) > 1) {
			int n = readarg(line, args);
			// commandes speciales du shell
			if (stringeq(args[0], "exit")) {
				break;
			} else if (stringeq(args[0], "ls")) {
				if (n == 1) {
					List();
				} else {
					invalidArguments(args, n, 0);
				}
			} else if (stringeq(args[0], "cd")) {
				if (n == 2) {
					if (!ChDir(args[1])) {
						printf("could not cd `%s'\n", args[1]);
					}
				} else if (n == 1) {
					ChDir("/");
				} else {
					invalidArguments(args, n, 1);
				}
			} else if (stringeq(args[0], "mkdir")) {
				if (n == 2) {
					if (!MkDir(args[1])) {
						printf("could not mkdir `%s'\n", args[1]);
					}
				} else {
					invalidArguments(args, n, 1);
				}
			} else if (stringeq(args[0], "rm")) {
				if (n == 2) {
					if (!Rm(args[1])) {
						printf("could not rm `%s'\n", args[1]);
					}
				} else {
					invalidArguments(args, n, 1);
				}
			} else {
				// programme dans le dossier courant
				pid = ForkExec(args[0]);
				if (pid == -1) {
					printf("Unable to execute %s\n", args[0]);
				} else {
					WaitPid(pid);
				}
			}
		}
	}

	return 0;
}
