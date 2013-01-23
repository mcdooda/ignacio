#ifdef CHANGED

#include "filesys.h"
#include "userfile.h"

#include "synch.h"
#include <map>
#include <iostream>


extern FileSystem* fileSystem;

class UserFile {
public:

	UserFile(const char* fileName, int f) :
	fd(f) {
#ifdef FILESYS
		openFile = fileSystem->OpenPath(fileName);
		absolutePath = fileSystem->GetAbsolutePath(fileName);
		std::cout << "Creation UserFile :<" << fileName << "> path : <" << absolutePath << ">" << std::endl;
#endif
	}

	void SetFd(int f) {
		fd = f;
	}

	int GetFd() {
		return fd;
	}

	std::string GetPath() {
		return absolutePath;
	}

private:
	int fd;
	OpenFile* openFile;
	std::string absolutePath;
};

Semaphore sem("UserFile", 1);
static int nextFd = 1;
static std::map<int, UserFile*> userFiles;
static std::map<std::string, UserFile*> userFilesPath;

void do_Create(const char* fileName) {
	bool b = fileSystem->Create(fileName, 200);
	if (!b) {
		std::cout << "Problème lors de la création du fichier " << fileName << std::endl;
	} else {
		std::cout << "Le fichier <" << fileName << "> a correctement été créé." << std::endl;
	}
}

int do_Open(const char* fileName) {
	sem.P();
	int fd = nextFd;
	nextFd++;
	UserFile* uF = new UserFile(fileName, fd);
	userFiles[fd] = uF;
	userFilesPath[std::string(fileName)] = uF;
	std::cout << "path = <" << uF->GetPath() << ">" << std::endl;
	sem.V();
	return fd;
}

int do_Read(int fd, void* buf, int count) {
	return 0;
}

int do_Write(int fd, void* buf, int count) {
	return 0;
}

int do_Close(int fd) {
	sem.P();
	int err = 0;
	std::map<int, UserFile*>::iterator it = userFiles.find(fd);
	if (it != userFiles.end()) {
		UserFile* uf = it->second;
		userFiles.erase(it);
		delete uf;
	} else {
		err = -1;
	}
	sem.V();
	return err;
}

#endif
