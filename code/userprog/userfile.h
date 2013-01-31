#ifdef CHANGED

#ifndef USERFILE_H
#define	USERFILE_H


extern void do_Create(const char* fileName);
extern int do_Open(int pid, const char* fileName);
extern int do_Read(int pid, int fd, char* buf, int count);
extern int do_Write(int pid, int fd, char* buf, int count);
extern int do_Close(int pid, int fd);
extern int do_Seek(int fd, int offset, int whence);
extern bool do_MkDir(const char* directory);

#endif	/* USERFILE_H */

#endif
