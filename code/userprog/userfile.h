#ifdef CHANGED

#ifndef USERFILE_H
#define	USERFILE_H


extern void do_Create(const char* fileName);
extern int do_Open(const char* fileName);
extern int do_Read(int fd, char* buf, int count);
extern int do_Write(int fd, char* buf, int count);
extern int do_Close(int fd);

#endif	/* USERFILE_H */

#endif
