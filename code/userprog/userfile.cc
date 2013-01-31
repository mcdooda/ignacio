#ifdef CHANGED

#include "filesys.h"
#include "userfile.h"
#include "userprocessus.h"
#include "synch.h"
#include "synchmap.h"
#include <iostream>
#include "syscall.h"

extern FileSystem* fileSystem;

class UserFile {
public:

	UserFile(const char* fileName, int f) :
	fd(f),
	numRef(1) {
#ifdef FILESYS
		openFile = fileSystem->OpenPath(fileName);
		absolutePath = fileSystem->GetAbsolutePath(fileName);
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

	std::string GetAbsolutePath() {
		return absolutePath;
	}

	void AddRef() {
		numRef++;
	}

	void RemoveRef() {
		numRef--;
	}

	int GetNumRef() {
		return numRef;
	}

private:
	int fd;
	int numRef;
	OpenFile* openFile;
	std::string absolutePath;
};

static int nextFd = 1;
static SynchMap<int, UserFile*> userFiles;
static SynchMap<std::string, UserFile*> userFilesPath;

void do_Create(const char* fileName) {
	//TODO peut etre mettre un userFilesaphore pour protéger le directory
	bool b = fileSystem->Create(fileName, 0);
	if (!b) {
		std::cout << "Problème lors de la création du fichier " << fileName << std::endl;
	} else {
		//		std::cout << "Le fichier <" << fileName << "> a correctement été créé." << std::endl;
	}
}

int do_Open(int pid, const char* fileName) {
	userFiles.P();
	//	userFiles.Dump();
	//	userFilesPath.Dump();
	int fd;
	UserFile* uf;
	if (userFilesPath.TryGet(std::string(fileName), uf)) {
		//Incrementer compteur
		uf->AddRef();
		fd = uf->GetFd();
		//printf("Trouvé dans la tof\n");
	} else {
		fd = nextFd;
		nextFd++;
		UserFile* uF = new UserFile(fileName, fd);
		//std::cout << "path = <" << uF->GetAbsolutePath() << ">" << std::endl;
		userFiles.Add(fd, uF);
		userFilesPath.Add(uF->GetAbsolutePath(), uF);
	}
	AddFileProc(pid, fd);
	userFiles.V();
	return fd;
}

int do_Read(int pid, int fd, char* buf, int count) {
	userFiles.P();
	int numRead;
	if (CheckFileProc(pid, fd)) {
		numRead = userFiles.Get(fd)->GetOpenFile()->Read(buf, count);
	} else {
		numRead = 0;
	}
	userFiles.V();
	return numRead;
}

int do_Write(int pid, int fd, char* buf, int count) {
	userFiles.P();
	int numWritten;
	if (CheckFileProc(pid, fd)) {
		numWritten = userFiles.Get(fd)->GetOpenFile()->Write(buf, count);
	} else {
		numWritten = 0;
	}
	userFiles.V();
	return numWritten;
}

int do_Close(int pid, int fd) {
	userFiles.P();
	int err = 0;
	if (CheckFileProc(pid, fd)) {
		UserFile* uf;
		if (userFiles.TryGet(fd, uf)) {
			uf->RemoveRef();
			if (uf->GetNumRef() == 0) {
				userFilesPath.Erase(uf->GetAbsolutePath());
				userFiles.Erase(fd);
				delete uf;
			}
			RemoveFileProc(pid, fd);
		} else {
			err = -1;
		}
	} else {
		err = -1;
	}
	userFiles.V();
	return err;
}

int do_Seek(int fd, int offset, int whence) {
	userFiles.P();
	int err = 0;
	UserFile* uf;
	if (userFiles.TryGet(fd, uf)) {
		OpenFile* of = uf->GetOpenFile();
		switch (whence) {
			case SEEK_SET:
				of->Seek(offset);
				break;
			case SEEK_CUR:
				of->Seek(of->Tell() + offset);
				break;
			case SEEK_END:
				of->Seek(of->Length() + offset);
				break;
			default:
				err = -1;
		}
	} else {
		err = -1;
	}
	userFiles.V();
	return err;
}

bool do_MkDir(const char* directory) {
	userFiles.P();
	OpenFile* of = fileSystem->Open(directory);
	if (of == NULL) {
		fileSystem->CreateDirectory(directory);
		return true;
	} else {
		delete of;
		return false;
	}
	userFiles.V();
}

bool do_Rm(const char* file) {
	userFiles.P();
	OpenFile* of = fileSystem->Open(file);
	if (of != NULL) {
		delete of;
		fileSystem->Remove(file);
		return true;
	} else {
		return false;
	}
	userFiles.V();
}

#endif
