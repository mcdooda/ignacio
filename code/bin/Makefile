# Use regular make for this Makefile
#
# Makefile for:
#	coff2noff -- converts a normal MIPS executable into a Nachos executable
#	disassemble -- disassembles a normal MIPS executable 
#
# Copyright (c) 1992 The Regents of the University of California.
# All rights reserved.  See copyright.h for copyright notice and limitation 
# of liability and disclaimer of warranty provisions.

include ../Makefile.common

# If the host is big endian (SPARC, SNAKE, etc):
# change to (disassemble and coff2flat don't support big endian yet):
# CFLAGS= -I./ -I../threads -DHOST_IS_BIG_ENDIAN
# all: coff2noff

INCDIRS+= bin threads filesys machine

ifeq ($(NACHOS_ARCH),SPARC_ARCH)
CPPFLAGS += -DHOST_IS_BIG_ENDIAN
endif

all: coff2noff

# converts a COFF file to Nachos object format
coff2noff: coff2noff.o

# converts a COFF file to a flat address space (for Nachos version 2)
coff2flat: coff2flat.o

# dis-assembles a COFF file
disassemble: out.o opstrings.o

PROGRAMS=coff2noff out disassemble
