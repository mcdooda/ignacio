# use normal make for this Makefile
#
# Makefile for building user programs to run on top of Nachos
#
# Several things to be aware of:
#
#    Nachos assumes that the location of the program startup routine (the
# 	location the kernel jumps to when the program initially starts up)
#       is at location 0.  This means: start.o must be the first .o passed 
# 	to ld, in order for the routine "Start" to be loaded at location 0
#

# if you are cross-compiling, you need to point to the right executables
# and change the flags to ld and the build procedure for as

NACHOS_ROOT = ../../
NACHOS_SYS := $(shell $(NACHOS_ROOT)/bin/nachos_sys)
NACHOS_ARCH := $(shell $(NACHOS_ROOT)/bin/nachos_arch)

ifeq ($(NACHOS_SYS),SOLARIS_SYS)
# Solaris/PC
ifeq ($(NACHOS_ARCH),X86_ARCH)
GCCDIR	= /soft/enseignants/Raymond.Namyst/Intel/decstation-ultrix/bin
endif
# Solaris/Sparc
ifeq ($(NACHOS_ARCH),SPARC_ARCH)
GCCDIR	= /soft/enseignants/Raymond.Namyst/Sparc/decstation-ultrix/bin
endif
endif

ifeq ($(NACHOS_SYS),LINUX_SYS)
#Linux : Mettez ici le chemin d'acces au repertoire d'installation 
#        des binutils + gcc, par exemple :
GCCDIR	= /usr/local/xgcc/decstation-ultrix/bin
endif

LDFLAGS = -T script -N
ASFLAGS = 
CPPFLAGS = $(INCDIR) -DCHANGED


# if you aren't cross-compiling:
# GCCDIR =
# LDFLAGS = -N -T 0
# ASFLAGS =
# CPPFLAGS = -P $(INCDIR)


CC = $(GCCDIR)/gcc
AS = $(GCCDIR)/as
LD = $(GCCDIR)/ld

CPP = $(GCCDIR)/gcc -E -P
INCDIR =-I../userprog -I../threads
CFLAGS = -c $(INCDIR) -G 0

all: halt shell matmult sort

start.o: start.S ../userprog/syscall.h
	$(CPP) $(CPPFLAGS) start.S > strt.s
	$(AS) $(ASFLAGS) -o start.o strt.s
	rm strt.s

halt.o: halt.c
	$(CC) $(CFLAGS) -c halt.c
halt: halt.o start.o
	$(LD) $(LDFLAGS) start.o halt.o -o halt.coff
	../bin/coff2noff halt.coff halt

shell.o: shell.c
	$(CC) $(CFLAGS) -c shell.c
shell: shell.o start.o
	$(LD) $(LDFLAGS) start.o shell.o -o shell.coff
	../bin/coff2noff shell.coff shell

sort.o: sort.c
	$(CC) $(CFLAGS) -c sort.c
sort: sort.o start.o
	$(LD) $(LDFLAGS) start.o sort.o -o sort.coff
	../bin/coff2noff sort.coff sort

matmult.o: matmult.c
	$(CC) $(CFLAGS) -c matmult.c
matmult: matmult.o start.o
	$(LD) $(LDFLAGS) start.o matmult.o -o matmult.coff
	../bin/coff2noff matmult.coff matmult

# Cleaning rule
.PHONY: clean
clean:
	rm -f core *.o *.s
