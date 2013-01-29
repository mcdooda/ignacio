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

	~UserFile() {
		delete openFile;
	}

	void SetFd(int f) {
		fd = f;
	}

	int GetFd() {
		return fd;
	}

	OpenFile* GetOpenFile() {
		return openFile;
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
	//TODO peut etre mettre un semaphore pour protéger le directory
	bool b = fileSystem->Create(fileName, 0);
	if (!b) {
		std::cout << "Problème lors de la création du fichier " << fileName << std::endl;
	} else {
		//		std::cout << "Le fichier <" << fileName << "> a correctement été créé." << std::endl;
	}
}

int do_Open(const char* fileName) {
	sem.P();
	
	//TODO je ne sais pas faire
	
	int fd;
//	bool inMap = false;
//	std::map<std::string, UserFile*>::iterator it;
//	std::string name = fileName;
//	for (it = userFilesPath.begin(); it != userFilesPath.end(); ++it) {
//		if (it->first == name) {
//			printf("nom trouvé <%s> nom cherché <%s> : fd = %d\n",it->first.c_str(), fileName, it->second->GetFd());
//			inMap = true;
//			fd = userFilesPath[std::string(fileName)]->GetFd();
//		}
//	}
//	if (!inMap) {
//	//TODO Fin de ce que je ne sais pas faire
		fd = nextFd;
		nextFd++;
		UserFile* uF = new UserFile(fileName, fd);
		userFiles[fd] = uF;
		userFilesPath[std::string(fileName)] = uF;
		//	std::cout << "path = <" << uF->GetPath() << ">" << std::endl;
//	}
	sem.V();
	printf("fd : %d dans la struct = %d\n",fd,userFilesPath[std::string(fileName)]->GetFd() );
	return fd;
}

int do_Read(int fd, char* buf, int count) {
	sem.P();
	int numRead = userFiles[fd]->GetOpenFile()->Read(buf, count);
	sem.V();
	return numRead;
}

int do_Write(int fd, char* buf, int count) {
	sem.P();
	//	printf("Do write\n");
	int numWritten = userFiles[fd]->GetOpenFile()->Write(buf, count);
	sem.V();
	return numWritten;
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
