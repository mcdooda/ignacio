#ifdef CHANGED

#ifndef USERFILE_H
#define	USERFILE_H


extern void do_Create(const char* fileName);
extern int do_Open(const char* fileName);
extern int do_Read(int fd, char* buf, int count);
extern int do_Write(int fd, char* buf, int count);
extern int do_Close(int fd);
extern int do_Seek(int fd, int offset, int whence);

#endif	/* USERFILE_H */

#endif
