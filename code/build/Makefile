# This GNU Makefile will compile several Nachos kernel and the MIPS user
# programs.

# All compilations in this directory is done *WITH* -DCHANGED

# This Makefile allows to compile several flavors of the Nachos kernel
# and also to (cross-)compile Nachos MIPS user programs.

# Compile only one flavor by using the target 'nachos-"flavor"'
# Example:    make nachos-threads
#
# Compile only MIPS user programs by using the target 'mips-progs'
# Example:    make mips-progs
#
# Compile only one MIPS user program by using the target 'mips-progs'
# and setting the PROGS variable
# Example:    make mips-progs PROGS=halt

#######################################################################
# Nachos kernels
# 
# By default, all USER_FLAVORS (see ../Makefile.define-flavors) are
# compiled. The compiled kernels are named 'nachos-"flavor"' and they
# include object files whose name is prefixed by '"flavor"-'
# Look into ../Makefile.define-flavors to define new flavors if needed.
#######################################################################
# MIPS user programs
#
# By default, all C files in test/ are expected to create a program.
# If this is not the case, change rules/depends in Makefile.compile-mips-progs
#######################################################################

include ../Makefile.common
include $(topsrc_dir)/Makefile.define-flavors

VARIANTS=filesys vm threads userprog network

ALL_NACHOS=$(addprefix nachos-,$(USER_FLAVORS))

all: $(ALL_NACHOS) mips-progs

mk_V = $(mk_V_$(V))
mk_V_ = $(mk_V_$(DEFAULT_VERBOSITY))
mk_V_0 = @printf "\nCompiling Nachos kernel for flavor $* into $@\n";
mk_V_1 = @printf "\n*** Compiling Nachos kernel for flavor $* into $@\n";

mm_V = $(mm_V_$(V))
mm_V_ = $(mm_V_$(DEFAULT_VERBOSITY))
mm_V_0 = @printf "\nCompiling MIPS Nachos user space programs\n";
mm_V_1 = @printf "\n*** Compiling MIPS Nachos user space programs\n";

.PHONY: $(ALL_NACHOS) mips-progs
$(ALL_NACHOS): nachos-%:
	$(mk_V)$(MAKE) --no-print-directory \
		FLAVOR=$* \
		DEFINES="$(DEFINES) -DCHANGED" \
		OBJ_PREFIX=$*- \
		NACHOS=$@ \
		-f $(topsrc_dir)/Makefile.common \
		-f $(topsrc_dir)/Makefile.compile-flavor \
		all

mips-progs:
	$(mm_V)$(MAKE) --no-print-directory \
		TARGET=MIPS \
		DEFINES="$(DEFINES) -DCHANGED" \
		-f $(topsrc_dir)/Makefile.common \
		-f $(topsrc_dir)/Makefile.compile-mips-progs \
		all

clean::
	$(RM) $(ALL_NACHOS)
	$(dep_V)$(MAKE) --no-print-directory \
		TARGET=MIPS \
		DEFINES="$(DEFINES) -DCHANGED" \
		-f $(topsrc_dir)/Makefile.common \
		-f $(topsrc_dir)/Makefile.compile-mips-progs \
		clean
