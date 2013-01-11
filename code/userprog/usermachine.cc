#ifdef CHANGED

#include "usermachine.h"
#include "machine.h"
#include "system.h"

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
		machine->WriteMem(adr, 1, ((char*) str)[i]);
		adr += sizeof (char);
	}
}

char UserMachine::GetCharArg(int arg) {
	return machine->ReadRegister(arg + 3) & 0xFF;
}

int UserMachine::GetIntArg(int arg) {
	return machine->ReadRegister(arg + 3);
}

void UserMachine::GetStringArg(int arg, char* str) {
	int addr = machine->ReadRegister(arg + 3);
	CopyStringFromMachine(addr, str, MAX_STRING_SIZE);
}

void UserMachine::SetOutArg(int arg, char c) {
	int addr = machine->ReadRegister(arg + 3);
	CopyDataToMachine(addr, &c, sizeof (char));
}

void UserMachine::SetOutArg(int arg, int i) {
	int addr = machine->ReadRegister(arg + 3);
	CopyDataToMachine(addr, &i, sizeof (int));
}

void UserMachine::SetOutArg(int arg, char* str) {
	int addr = machine->ReadRegister(arg + 3);
	int size = strlen(str);
	CopyStringToMachine(addr, str, size);
}

void UserMachine::SetReturn(char c) {
	machine->WriteRegister(2, c);
}

void UserMachine::SetReturn(int i) {
	machine->WriteRegister(2, i);
}

#endif // CHANGED

