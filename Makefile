SOURCES   := $(wildcard *.cc)
EXES      := $(SOURCES:.cc=)
CHECKS    := $(SOURCES:.cc=.check)

CCFLAGS   := -ggdb3 -O2 -Wall -Wextra -std=c++11 -Wfatal-errors
LDFLAGS   := -pthread -rdynamic 

CXX       := clang++


default: build

# --- build
build: $(EXES)

check: $(CHECKS)

info:
	@echo "SOURCES := $(SOURCES)"
	@echo "CHECKS  := $(CHECKS)"
	@echo "EXES    := $(EXES)"


%.run: %
	./$<

%.check: %.cc
	cppcheck --enable=all --std=c++11 --verbose $<

%.style: %.cc
	KWStyle -v -xml KWStyle.xml -gcc $<

# --------

%.o: %.cc
	$(CXX) $(CCFLAGS) -c -o $@ $<
%: %.o
	$(CXX) $(LDFLAGS) -o $@ $<
%.s: %.cc
	$(CXX) $(CCFLAGS) -fverbose-asm -S -o $@ $<
%.lst: %.s
	$(AS) -alhnd -o /dev/null $< > $@

#---------

clean:
	-rm $(EXES)
	-rm $(SOURCES:.cc=.o)
	-rm $(SOURCES:.cc=.s) $(SOURCES:.cc=.lst)


.SUFFIXES:
.PHONY: default build clean
