# Copyright (c) 1992 The Regents of the University of California.
# All rights reserved.  See copyright.h for copyright notice and limitation 
# of liability and disclaimer of warranty provisions.

MAKE	:=	make
## LPR	:=	echo
LPR	:=	enscript

DIRS	:=	threads userprog filesys network vm

.PHONY: all clean depend print

all:
	@set -e; \
	for i in $(DIRS); do \
		$(MAKE) -C $$i nachos; \
	done ; \
	for i in bin test; do \
		$(MAKE) -C $$i all; \
	done

clean:
	@set -e; \
	for i in $(DIRS) bin test; do \
		$(MAKE) -C $$i $@; \
	done

depend:
	@set -e; \
	for i in $(DIRS); do \
		$(MAKE) -C $$i $@; \
	done

print:
	set -e; \
	for dir in machine $(DIRS) test; do \
		$(LPR) -Emakefile $$dir/Makefile \
			-Ecpp $${dir}/*.h $${dir}/*.cc $${dir}/*.S \
			-Ec $${dir}/*.c; \
	done
	$(LPR) -Emakefile Makefile*
	$(LPR) -Ec bin/noff.h bin/coff.h bin/coff2noff.c

INDENT	:= indent --indent-level4
DIRS_INDENT	:= threads userprog test

indent:
	set +e; \
	for dir in $(DIRS_INDENT); do \
		$(INDENT)  `ls $$dir/*.cc $$dir/*.h $$dir/*.c`; \
	done
