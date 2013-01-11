#ifdef CHANGED

#include "usermachine.h"
#include "machine.h"

extern Machine* machine;

void UserMachine::CopyStringFromMachine(int adr, char *str, int size) {
	for (int i = 0; i < size - 1; i++) {
		machine->ReadMem(adr, 1, (int*) &str[i]);
		adr += sizeof (char);
		if (str[i] == '\0') {
			break;
		}
	}
	str[size - 1] = '\0';
}

void UserMachine::CopyStringToMachine(int adr, char *str, int size) {
	for (int i = 0; i < size; i++) {
		machine->WriteMem(adr, 1, str[i]);
		adr += sizeof (char);
		if (str[i] == '\0') {
			break;
		}
	}
}

void UserMachine::CopyDataToMachine(int adr, void *str, int size) {
	for (int i = 0; i < size; i++) {
		machine->WriteMem(adr, 1, ((char*)str)[i]);
		adr += sizeof (char);
	}
}

#endif // CHANGED

