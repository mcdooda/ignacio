# NOTE: this is a GNU Makefile.  You must use "gmake" rather than "make".
#
# Makefile for the virtual memory assignment
#    Defines set up assuming the virtual memory assignment is done before 
# 	the file system assignment. If not, use the "filesys first"  
#	defines in ../Makefile.define-flavors. 
#
# Also, if you want to simplify the translation so it assumes
# only linear page tables, don't define USE_TLB.
#
# Copyright (c) 1992 The Regents of the University of California.
# All rights reserved.  See copyright.h for copyright notice and limitation 
# of liability and disclaimer of warranty provisions.

# This makefile will compile the flavor named as the current directory
# This compilation is done *WITHOUT* -DCHANGE (look into the build/
# directory to have a build with -DCHANGE)

FLAVOR=$(notdir $(CURDIR))

include ../Makefile.common
include ../Makefile.compile-flavor
