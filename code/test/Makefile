# This Makefile (cross-)compiles Nachos MIPS user programs
#
# In this directory, compilation is done *WITHOUT* -DCHANGED
# Use the build/ directory to build these programs with -DCHANGED
#
# By default, all C files are expected to create a program.
# If this is not the case, change rules/depends in Makefile.compile-mips-progs

ifneq ($(MAKECMDGOALS),clean)
$(warning Compiling nachos user programs *without* user modifications (without -DCHANGED))
endif

TARGET=MIPS
include ../Makefile.common
include $(topsrc_dir)/Makefile.compile-mips-progs

