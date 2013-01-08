#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;
static void ReadAvail(int arg) { readAvail->V(); }
static void WriteDone(int arg) { writeDone->V(); }

SynchConsole::SynchConsole(char *readFile, char *writeFile) :
	Console(readFile, writeFile, &::ReadAvail, &::WriteDone, 0)
{
	readAvail = new Semaphore("read avail", 0);
	writeDone = new Semaphore("write done", 0);
// 	console = ...
}

SynchConsole::~SynchConsole() {
// 	delete console;
	delete writeDone;
	delete readAvail;
}

void SynchConsole::SynchPutChar(const char ch) {
	PutChar(ch);
	writeDone->P();
}

char SynchConsole::SynchGetChar() {
	readAvail->P ();
	return GetChar ();
}

void SynchConsole::SynchPutString(const char s[]) {
	for (int i = 0; s[i] != '\0'; i++)
		SynchPutChar(s[i]);
}

void SynchConsole::SynchGetString(char *s, int n) {
	for (int i = 0; i < n; i++)
		s[i] = SynchGetChar();
}

#endif // CHANGED
