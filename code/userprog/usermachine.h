#ifdef CHANGED

#ifndef USERMACHINE_H
#define	USERMACHINE_H

class UserMachine {
public:
	void CopyStringFromMachine(int addr, char *str, int size);
	void CopyStringToMachine(int addr, char *str, int size);
	void CopyDataFromMachine(int addr, void *data, int size);
	void CopyDataToMachine(int addr, void *data, int size);
	
	char GetCharArg(int arg);
	int GetIntArg(int arg);
	void GetStringArg(int arg, char* str);
	
	void SetOutArg(int arg, char c);
	void SetOutArg(int arg, int i);
	void SetOutArg(int arg, char* str);
	
	void SetReturn(char c);
	void SetReturn(int i);
};

#endif	/* USERMACHINE_H */

#endif // CHANGED
