# Copyright (c) 1992 The Regents of the University of California.
# All rights reserved.  See copyright.h for copyright notice and limitation 
# of liability and disclaimer of warranty provisions.

MAKE	:=	make
LPR	:=	lpr

DIRS	:=	threads userprog filesys network vm

.PHONY: all clean depend print

all:
	@set -e; \
	for i in $(DIRS) ; do \
		$(MAKE) -C $$i nachos; \
	done ; \
	for i in bin test ; do \
		$(MAKE) -C $$i all; \
	done

clean:
	@set -e; \
	for i in $(DIRS) bin test ; do \
		$(MAKE) -C $$i $@; \
	done

depend:
	@set -e; \
	for i in $(DIRS) ; do \
		$(MAKE) -C $$i $@; \
	done

print:
	@set -e; \
	for i in machine $(DIR) test ; do \
		$(LPR) Makefile *.h *.cc *.S *.c ; \
	done
	@$(LPR) Makefile*
	$(LPR) bin/noff.h bin/coff.h bin/coff2noff.c

