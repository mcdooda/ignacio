#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;
static Semaphore *semPutChar;
static Semaphore *semGetChar;
static Semaphore *semPutString;
static Semaphore *semGetString;

static void ReadAvail(int arg) {
	readAvail->V();
}

static void WriteDone(int arg) {
	writeDone->V();
}

SynchConsole::SynchConsole(char *readFile, char *writeFile) :
Console(readFile, writeFile, &::ReadAvail, &::WriteDone, 0) {
	readAvail = new Semaphore("read avail", 0);
	writeDone = new Semaphore("write done", 0);
	semGetChar = new Semaphore("getChar", 1);
	semPutChar = new Semaphore("putChar", 1);
	semGetString = new Semaphore("getString", 1);
	semPutString = new Semaphore("putString", 1);
}

SynchConsole::~SynchConsole() {
	delete writeDone;
	delete readAvail;
	delete semGetChar;
	delete semPutChar;
	delete semGetString;
	delete semPutString;
}

void SynchConsole::SynchPutChar(const char ch) {
	semPutChar->P();
	PutChar(ch);
	writeDone->P();
	semPutChar->V();
}

int SynchConsole::SynchGetChar() {
	readAvail->P();
	semGetChar->P();
	unsigned char c;
	if (feof()) {
		c = EOF;
	} else {
		c = GetChar();
	}
	semGetChar->V();
	return c;
}

void SynchConsole::SynchPutString(const char s[]) {
	semPutString->P();
	for (int i = 0; s[i] != '\0'; i++) {
		SynchPutChar(s[i]);
	}
	semPutString->V();
}

void SynchConsole::SynchGetString(char *s, int n) {
	semGetString->P();
	for (int i = 0; i < n - 1; i++) {
		int c = SynchGetChar();
		if (c == '\n') {
			s[i] = '\n';
			s[i + 1] = '\0';
			break;
		} else if (c == '\0' || c == EOF) {
			s[i] = '\0';
			break;
		} else {
			s[i] = c;
		}
	}
	s[n - 1] = '\0';
	semGetString->V();
}


#endif // CHANGED
