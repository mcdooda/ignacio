# Use regular make for this Makefile
#
# Makefile for building user programs to run on top of Nachos
# Several things to be aware of: Nachos assumes that the location of
# the program startup routine (the location where the kernel jumps to
# when the program initially starts up) is location 0.  This means:
# start.o must be the first .o passed to ld, so that routine "Start"
# gets loaded at location 0
#
# if you are cross-compiling, you need to point to the right executables
# and change the flags to "ld" and the build procedure for "as".

NACHOS_ROOT = ../../
NACHOS_SYS := $(shell $(NACHOS_ROOT)/bin/nachos_sys)
NACHOS_ARCH := $(shell $(NACHOS_ROOT)/bin/nachos_arch)

ifeq ($(NACHOS_SYS),SOLARIS_SYS)
# Solaris/PC
ifeq ($(NACHOS_ARCH),X86_ARCH)
GCCDIR	= /opt/NACHOS/nachos_gcc/cross/decstation-ultrix/bin
endif
# Solaris/Sparc
ifeq ($(NACHOS_ARCH),SPARC_ARCH)
GCCDIR	= /opt/NACHOS/nachos_gcc/cross/decstation-ultrix/bin
endif
endif

ifeq ($(NACHOS_SYS),LINUX_SYS)
# Linux: fill in with the full path to the binutils and gcc.
# For instance:
GCCDIR	= /opt/NACHOS/nachos_gcc/cross/decstation-ultrix/bin
endif

LDFLAGS = -T script -N
ASFLAGS = 
CPPFLAGS = $(INCDIR) -DCHANGED


# If you aren't cross-compiling:
# GCCDIR =
# LDFLAGS = -N -T 0
# ASFLAGS =
# CPPFLAGS = -P $(INCDIR)


CC	:=	$(GCCDIR)/gcc
AS	:=	$(GCCDIR)/as
LD	:=	$(GCCDIR)/ld

CPP	:=	$(GCCDIR)/gcc -E -P
INCDIR	:=	-I../userprog -I../threads
# LB: Missing options in CFLAGS: -Wall -DCHANGED
# CFLAGS 	:=	-DIN_USER_MODE -c $(INCDIR) -G 0 
CFLAGS 	:=	-DIN_USER_MODE -c $(INCDIR) -G 0 -Wall -DCHANGED
# End of modification

SOURCES	:=	$(wildcard *.c)
PROGS	:=	$(patsubst %.c,%,$(SOURCES))

# RN: Be careful to use '=' instead of ':=' here!
COFF	=	$(patsubst %,%.coff,$@)

.PHONY: all

all: $(PROGS)

start.o: start.S ../userprog/syscall.h
	$(CPP) $(CPPFLAGS) start.S > strt.s
	$(AS) $(ASFLAGS) -o start.o strt.s
	rm strt.s

# LB: Missing dependency from syscall.h
# %.o: %.c
%.o: %.c ../userprog/syscall.h
# End of modification
	$(CC) $(CFLAGS) -c $<

# LB: Caution! start.o should appear *before* $< for the load!

$(PROGS): %: %.o start.o
	$(LD) $(LDFLAGS) start.o $< -o $(COFF)
	../bin/coff2noff $(COFF) $@
	rm $(COFF)

$(PROGS): start.o

# Cleaning rule
.PHONY: clean
clean:
	rm -f core *.o *.s $(PROGS)
