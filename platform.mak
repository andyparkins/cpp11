# ----------------------------------------------------------------------------
# Version Control
#    $Author: andyp $
#      $Date$
#        $Id$
#
# Legal
#    Copyright 2013  FussyLogic Ltd
#
# ----------------------------------------------------------------------------

# This makefile fragment automatically sets some variables that help with
# compiling a cross-platform project.

# There are four possibilities to be dealt with:
#   - Linux host,   Compiling for Linux platform
#   - Linux host,   Compiling for Windows platform (pass PLATFORM=win32)
#   - Windows host, Compiling for Windows platform
#   - Windows MSYS host, Compiling for Windows platform
# This section sets up the right variables for each case ready for the rest of
# the makefile.
# First, we must detect a Windows host
ifneq ($(WINDIR)$(windir),)
#   In cygwin, the variable $(WINDIR) exists.  From a command line running
#   mingw32-make $(windir) exists.  The presence of either means that we are
#   compiling on windows.
    PLATFORMBUILDHOST           := win32
    ifneq ($(MSYSTEM),)
#       On an MSYS system, MSYSTEM is set
        PLATFORMMAKEMODE        := unix
    else
        PLATFORMMAKEMODE        := win32
    endif
else
    PLATFORMBUILDHOST           := unix
    PLATFORMMAKEMODE            := unix
endif

CC  ?= gcc
CXX ?= g++

HOSTCXX            := $(CXX)
HOSTCC             := $(CC)
HOSTCPP            := cpp
HOSTADDR2LINE      := addr2line
HOSTAR             := ar
HOSTAS             := as
HOSTCXXFILT        := c++filt
HOSTCPP            := cpp
HOSTDLLTOOL        := dlltool
HOSTDLLWRAP        := dllwrap
HOSTGCCBUG         := gccbug
HOSTGCOV           := gcov
HOSTGPROF          := gprof
HOSTLD             := ld
HOSTNM             := nm
HOSTOBJCOPY        := objcopy
HOSTOBJDUMP        := objdump
HOSTRANLIB         := ranlib
HOSTREADELF        := readelf
HOSTSIZE           := size
HOSTSTRINGS        := strings
HOSTSTRIP          := strip
HOSTWINDMC         := windmc
HOSTWINDRES        := windres

# Host-specific settings
ifeq ($(PLATFORMBUILDHOST),win32)
#   The only possible target on Windows is Windows
    PLATFORM          := win32
    ifeq ($(PLATFORMMAKEMODE),unix)
        RM            := rm -f
        COPY          := cp
        COPY_DIR      := cp -r
        MSPWD         := $(shell cmd //c echo $(shell pwd) | tr / \\)
    else
        RM            := del
        COPY          := copy /y
        COPY_DIR      := xcopy /x /q /y /i
        MSPWD         := $(shell cd)
    endif
else
    RM             := rm -f
#   If PLATFORMBUILDHOST isn't win32, then we're compiling on UNIX, in that
#   case, if $PLATFORM is win32 we want to use the mingw32 cross compiler
    ifeq ($(PLATFORM),win32)
        BINUTILSPREFIX := i586-mingw32msvc-
    else
#       Building on UNIX for UNIX
        BINUTILSPREFIX ?=
    endif
    CXX            := $(BINUTILSPREFIX)$(CXX)
    CC             := $(BINUTILSPREFIX)$(CC)
    CPP            := $(BINUTILSPREFIX)cpp
    ADDR2LINE      := $(BINUTILSPREFIX)addr2line
    AR             := $(BINUTILSPREFIX)ar
    AS             := $(BINUTILSPREFIX)as
    CXXFILT        := $(BINUTILSPREFIX)c++filt
    CPP            := $(BINUTILSPREFIX)cpp
    DLLTOOL        := $(BINUTILSPREFIX)dlltool
    DLLWRAP        := $(BINUTILSPREFIX)dllwrap
    GCCBUG         := $(BINUTILSPREFIX)gccbug
    GCOV           := $(BINUTILSPREFIX)gcov
    GPROF          := $(BINUTILSPREFIX)gprof
    LD             := $(BINUTILSPREFIX)ld
    NM             := $(BINUTILSPREFIX)nm
    OBJCOPY        := $(BINUTILSPREFIX)objcopy
    OBJDUMP        := $(BINUTILSPREFIX)objdump
    RANLIB         := $(BINUTILSPREFIX)ranlib
    READELF        := $(BINUTILSPREFIX)readelf
    SIZE           := $(BINUTILSPREFIX)size
    STRINGS        := $(BINUTILSPREFIX)strings
    STRIP          := $(BINUTILSPREFIX)strip
    WINDMC         := $(BINUTILSPREFIX)windmc
    WINDRES        := $(BINUTILSPREFIX)windres
endif

