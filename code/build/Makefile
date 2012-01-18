# Copyright (C) 2011 Vincent Danjean <Vincent.Danjean@imag.fr>
#
# This Makefile try to recompile the nachos *with* -DCHANGED
#
# All NachOS flavors listed in USER_FLAVORS are compiled
# See Makefile.define-user to add/remove flavors to this list
#
# All NachOS user programs are compiled. By default, there is one
# program per C source file in test/ directory.
# See Makefile.define-user to add/remove NachOS user programs

AUTOLOAD_USERPROGS=all
AUTOLOAD_KERNELS=$(USER_FLAVORS)

AUTOLOAD_CPPFLAGS=-DCHANGED

include ../Makefile.rules-nachos
