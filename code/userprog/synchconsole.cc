#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;
static Semaphore *getString;
static void ReadAvail(int arg) { readAvail->V(); }
static void WriteDone(int arg) { writeDone->V(); }

SynchConsole::SynchConsole(char *readFile, char *writeFile) :
	Console(readFile, writeFile, &::ReadAvail, &::WriteDone, 0)
{
	readAvail = new Semaphore("read avail", 0);
	writeDone = new Semaphore("write done", 0);
	getString = new Semaphore("get string", 1);
}

SynchConsole::~SynchConsole() {
	delete writeDone;
	delete readAvail;
	delete getString;
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
	getString->P();
	int i;
	for (i = 0; i < n - 1; i++){
		s[i] = SynchGetChar();
		if(s[i] == '\n')
			break;
	}
	s[i+1] = '\0';
	getString->V();
}

bool SynchConsole::feof()
{
//	return incoming != EOF && !PollFile(readFileNo);
	return false;
}

#endif // CHANGED
