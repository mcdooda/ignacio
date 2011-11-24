# Copyright (c) 1992 The Regents of the University of California.
# All rights reserved.  See copyright.h for copyright notice and limitation 
# of liability and disclaimer of warranty provisions.

DIRS	:=	threads userprog filesys network vm

.PHONY: all clean depend print build check mips-progs

all: build

build:
	$(MAKE) -C build DEFAULT_VERBOSITY=0

mips-progs:
	$(MAKE) -C build DEFAULT_VERBOSITY=0 $@

check: build
	@set -e; \
	for i in $(DIRS); do \
		$(MAKE) -C $$i nachos; \
	done ; \
	for i in bin test; do \
		$(MAKE) -C $$i all; \
	done

clean:
	@set -e; \
	for i in build $(DIRS) bin test; do \
		$(MAKE) -C $$i $@; \
	done

## LPR	:=	echo
LPR	:=	enscript --columns=2  --borders -r --landscape \
		--media=A4 --verbose --pretty-print --toc 

print:
	-for dir in machine threads userprog filesys network vm test; \
	do \
		ls -f \
		$$dir/Makefile* \
		$${dir}/*.h \
		$${dir}/*.cc $${dir}/*.S \
		$${dir}/*.c; \
	done > list
	$(LPR) `cat list` 

INDENT	:= indent --indent-level4
DIRS_INDENT	:= threads userprog test

indent:
	-for dir in machine $(DIRS_INDENT) test; do \
		ls -f \
		$${dir}/*.h \
		$${dir}/*.cc \
		$${dir}/*.c; \
	done > list
	for file in `cat list`; do \
		echo $${file}; $(INDENT) $${file}; \
	done
