# Copyright (c) 1992 The Regents of the University of California.
# All rights reserved.  See copyright.h for copyright notice and limitation 
# of liability and disclaimer of warranty provisions.

MAKE = make
LPR = lpr

.PHONY: all clean depend print

all: 
	cd threads; $(MAKE) nachos
	cd userprog; $(MAKE) nachos 
	cd vm; $(MAKE) nachos 
	cd filesys; $(MAKE) nachos 
	cd network; $(MAKE) nachos 
	cd bin; make all
	cd test; make all

clean:
	rm -f `find . -name 'core' -o -name 'nachos' -o -name 'DISK' -o \
		-name '*.o' -o -name '*.s' -o -name '*.coff' -o \
		-name 'coff2flat' -o -name 'coff2noff' -o -name 'out' -o \
		-name 'disassemble' -o -name '.depend'`

depend:
	cd threads; $(MAKE) depend
	cd userprog; $(MAKE) depend 
	cd vm; $(MAKE) depend
	cd filesys; $(MAKE) depend
	cd network; $(MAKE) depend

print:
	/bin/csh -c "$(LPR) Makefile* */Makefile"
	/bin/csh -c "$(LPR) threads/*.h threads/*.cc threads/*.s"
	/bin/csh -c "$(LPR) userprog/*.h userprog/*.cc" 
	/bin/csh -c "$(LPR) filesys/*.h filesys/*.cc
	/bin/csh -c "$(LPR) network/*.h network/*.cc 
	/bin/csh -c "$(LPR) machine/*.h machine/*.cc
	/bin/csh -c "$(LPR) bin/noff.h bin/coff.h bin/coff2noff.c"
	/bin/csh -c "$(LPR) test/*.h test/*.c test/*.s"
