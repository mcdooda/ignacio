#ifdef CHANGED

#include "usermachine.h"
#include "machine.h"
#include "system.h"

extern Machine* machine;

void UserMachine::CopyStringFromMachine(int addr, char *str, int size) {
	for (int i = 0; i < size - 1; i++) {
		machine->ReadMem(addr, 1, (int*) &str[i]);
		addr += sizeof (char);
		if (str[i] == '\0') {
			break;
		}
	}
	str[size - 1] = '\0';
}

void UserMachine::CopyStringToMachine(int addr, char *str, int size) {
	for (int i = 0; i < size; i++) {
		machine->WriteMem(addr, 1, str[i]);
		addr += sizeof (char);
		if (str[i] == '\0') {
			break;
		}
	}
}

void UserMachine::CopyDataFromMachine(int addr, void *data, int size) {
	for (int i = 0; i < size; i++) {
		//machine->ReadMem(addr, 1, (int*)((char*)data)[i]);
		addr += sizeof (char);
	}
}

void UserMachine::CopyDataToMachine(int addr, void *data, int size) {
	for (int i = 0; i < size; i++) {
		machine->WriteMem(addr, 1, ((char*)data)[i]);
		addr += sizeof (char);
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
	int size = strlen(str) + 1;
	CopyStringToMachine(addr, str, size);
}

void UserMachine::SetReturn(char c) {
	machine->WriteRegister(2, c);
}

void UserMachine::SetReturn(int i) {
	machine->WriteRegister(2, i);
}

#endif // CHANGED

