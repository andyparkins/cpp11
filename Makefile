# ----------------------------------------------------------------------------
# Version Control
#    $Author$
#      $Date$
#        $Id: 2f94cac8bd3d6b9a7f5f2f0992b3cfdd344b5d94 $
#
# Legal
#    Copyright 2013 FussyLogic Ltd
#
# ----------------------------------------------------------------------------

# parameters
CXX        := clang++
HOSTCXX    := clang++
EXES       ?=

# constants
CSOURCES   := $(wildcard *.c)
CCSOURCES  := $(wildcard *.cc)
CCCHECKS   := $(CCSOURCES:.cc=.check)
OBJS       := $(CCSOURCES:.cc=.o)
OBJMAKS    := $(OBJS:.o=.mak)
CCFLAGS    := -Wall -Wextra -Wfatal-errors -fmessage-length=0 -std=c++11 -ggdb3 -O3 $(CCFLAGS)
LDFLAGS    := -fmessage-length=0 -pthread -rdynamic $(LDFLAGS)
INCLUDEDCLEANS :=
# toolchain constants
include platform.mak

# -----

default: build

include revision.mak

# --- build

build: $(EXES)

check: $(CCCHECKS)
xmlcheck: $(CCCHECKS:.check=.xmlcheck)

info:
	@echo "--- INFO (make)"
	@echo "SOURCES    = $(CCSOURCES) $(CSOURCES)"
	@echo "CHECKS     = $(CCCHECKS)"
	@echo "EXES       = $(EXES)"
	@echo "PLATFORM   = $(PLATFORM)"
	@echo "REVISION   = $(VERSIONSTRINGRAW)"
	@echo "OBJS       = $(OBJS)"
	@echo "VERS       = $(TAGGEDSOVERSION) / $(TAGGEDSOVERSIONL)"
	@echo "--- INFO (end)"

# --- recipes

# Per-module Makefile components
-include $(OBJS:.o=.mak)

%.o: %.cc
	$(CXX) $(CCFLAGS) -c -o $@ $<
%: %.o
	$(CXX) $(LDFLAGS) -o $@ $<
%.s: %.cc
	$(CXX) $(CCFLAGS) -fverbose-asm -S -o $@ $<
%.lst: %.s
	$(AS) -alhnd -o /dev/null $< > $@

-include dynamic.mak

# --- code checks

-include unittest.mak

%.check: %.cc
	cppcheck --enable=all --std=c++11 --verbose $<
%.xmlcheck: %.cc
	cppcheck --enable=all --std=c++11 --xml --verbose $< 2> $@
%.style: %.cc
	KWStyle -v -xml KWStyle.xml -gcc $<

# --- utility

clean: $(INCLUDEDCLEANS)
	-$(RM) $(EXES)
	-$(RM) $(CCSOURCES:.cc=.o) $(CSOURCES:.cc=.o)
	-$(RM) $(CCSOURCES:.cc=.s) $(CCSOURCES:.cc=.lst) $(CSOURCES:.cc=.s) $(CSOURCES:.cc=.lst)


.SUFFIXES:
.PHONY: default build clean info $(INCLUDEDCLEANS)

