# Copyright (c) 2011 Vincent Danjean <Vincent.Danjean@imag.fr>

DEFAULT_VERBOSITY=0
export DEFAULT_VERBOSITY

.PHONY: all clean depend print build build-origin origin check mips-progs bin

# By default, build the check target so that we check that initial
# nachos code still compile
check:

all: build

build build-origin: bin
	$(MAKE) -C $@

bin:
	$(MAKE) -C bin

mips-progs:
	$(MAKE) -C build $@

check: origin build

origin:
	@set -e; RET= ;\
	$(MAKE) -C build-origin || RET="1" ;\
	if [ -n "$$RET" ]; then \
		echo "=========================================================================" ;\
		echo "=========================================================================" ;\
		echo "=========================================================================" ;\
		echo "Initial compilation of nachos does not work in the following directory:"   ;\
		echo "build-origin" ;\
		echo "Did you forget to protect your modifications with '#ifdef CHANGED'?" ;\
		echo "=========================================================================" ;\
		echo "=========================================================================" ;\
		echo "=========================================================================" ;\
		exit 1;\
	fi

clean:
	@set -e; \
	for i in build build-origin bin; do \
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
