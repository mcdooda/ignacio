# NOTE: this is a GNU Makefile.  You must use "gmake" rather than "make".
#
# Makefile for the multiprogramming assignment
#    Defines set up assuming multiprogramming is done before the file system.
#    If not, use the "filesys first"  defines below. 
#
#
# Copyright (c) 1992 The Regents of the University of California.
# All rights reserved.  See copyright.h for copyright notice and limitation 
# of liability and disclaimer of warranty provisions.

DEFINES = -DUSER_PROGRAM -DFILESYS_NEEDED -DFILESYS_STUB
INCPATH = -I../bin -I../filesys -I../userprog -I../threads -I../machine
C_OFILES = $(THREAD_O) $(USERPROG_O)

# if file system done first!
# DEFINES = -DUSER_PROGRAM -DFILESYS_NEEDED -DFILESYS
# INCPATH = -I../bin -I../filesys -I../userprog -I../threads -I../machine
# C_OFILES = $(THREAD_O) $(USERPROG_O) $(FILESYS_O)

include ../Makefile.common
include ../Makefile.dep


