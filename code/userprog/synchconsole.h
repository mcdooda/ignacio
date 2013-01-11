// synchconsole.h 

#ifdef CHANGED

#ifndef SYNCHCONSOLE_H
#define SYNCHCONSOLE_H

#include "copyright.h"
#include "utility.h"
#include "console.h"


class SynchConsole : public Console {
	public:
		// initialize the hardware console device
		SynchConsole(char *readFile, char *writeFile);

		// clean up console emulation
		~SynchConsole();
		
		// Unix putchar(3S)
		void SynchPutChar(const char ch);
		
		// Unix getchar(3S)
		int SynchGetChar();
		
		// Unix puts(3S)
		void SynchPutString(const char *s);
		
		// Unix fgets(3S)
		void SynchGetString(char *s, int n);
		
};


#endif // SYNCHCONSOLE_H

#endif // CHANGED
