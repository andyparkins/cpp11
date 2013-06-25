SOURCES   := $(wildcard *.cc)
EXES      := $(SOURCES:.cc=)

CCFLAGS   := -ggdb3 -O2 -Wall -Wextra -std=c++11 -Wfatal-errors
LDFLAGS   := -pthread -rdynamic 

CXX       := clang++


default: build

# --- build
build: $(EXES)

info:
	@echo "SOURCES := $(SOURCES)"
	@echo "EXES    := $(EXES)"


%.run: %
	./$<

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
