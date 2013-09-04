# ----------------------------------------------------------------------------
# Version Control
#    $Author: andyp $
#      $Date$
#        $Id: ee8ba619baed17e9049d2162a563039f7ae00270 $
#
# Legal
#    Copyright 2013  FussyLogic Ltd
#
# Notes
#  UNITTESTs are expected to be main() implementations in library modules
#  that run a self-test of the functionality in that module.
#
#  That main() should be wrapped in a #ifdef UNITTEST to be detected by
#  this Makefile.
#
# ----------------------------------------------------------------------------

UNITTEST_PKGLIBS  := cppunit
UNITTEST_SOURCES  ?= $(CCSOURCES)
UNITTEST_LIBS     ?= $(LIBS)
UNITTEST_LIBPATH  ?= $(LIBPATH)
UNITTEST_INCLUDES ?= $(INCLUDES)
UNITTEST_LDFLAGS  ?= $(LDFLAGS) $(shell pkg-config --libs $(UNITTEST_PKGLIBS))
UNITTEST_CCFLAGS  ?= $(CCFLAGS) $(shell pkg-config --cflags $(UNITTEST_PKGLIBS))

# Testable modules are any that have 'ifdef UNITTEST' in them"
UNITTEST_SOURCES := $(shell grep -ls "ifdef UNITTEST" /dev/null $(UNITTEST_SOURCES))

# Expand to make convenience targets for the user to "make"
UNITTEST_EXES    := $(patsubst %.cc,unit-%,$(UNITTEST_SOURCES))

# Run a particular unit
test-%: unit-%
	@echo "[$*] ----- UNITTEST"
	@LD_LIBRARY_PATH=$(UNITTEST_LIBPATH):$${LD_LIBRARY_PATH} ./unit-$*
	@echo "[$*] ----- /UNITTEST"

# compile a particular unit.  This recipe automatically adds in per-module
# includes, libarary paths, include paths, libraries and defines.  UNITTEST
# is automatically defined, which the module can wrap its UNITTEST main() in.
# LDFLAGS and CCFLAGS are used as normal.
unit-%: %.cc
	$(HOSTCXX) $< $(UNITTEST_CCFLAGS) -O0 \
		$(patsubst %,"-D%",$($*_DEFINES)) -DUNITTEST \
		$(patsubst %,-I%,$(UNITTEST_INCLUDE)) $(patsubst %,-I%,$($*_INCLUDE)) \
		$(patsubst %,-L%,$(UNITTEST_LIBPATH)) $(patsubst %,-L%,$($*_LIBPATH)) \
		$(patsubst %,-l%,$(UNITTEST_LIBS)) $(patsubst %,-l%,$($*_LIBS)) \
		-o unit-$* $(UNITTEST_LDFLAGS)

unit-%: %.c
	$(HOSTCC) $< $(UNITTEST_CFLAGS) -O0 \
		$(patsubst %,"-D%",$($*_DEFINES)) -DUNITTEST \
		$(patsubst %,-I%,$(UNITTEST_INCLUDE)) $(patsubst %,-I%,$($*_INCLUDE)) \
		$(patsubst %,-l%,$(UNITTEST_LIBS)) $(patsubst %,-l%,$($*_LIBS)) \
		-o $@ $(UNITTEST_LDFLAGS)

# -------------
# Rely on cppunit for test registry
unit-%.o: %.cc
	$(HOSTCXX) $< $(UNITTEST_CCFLAGS) -O0 \
		$(patsubst %,"-D%",$($*_DEFINES)) -DUNITTEST -DUNITTESTALL \
		$(patsubst %,-I%,$(UNITTEST_INCLUDE)) $(patsubst %,-I%,$($*_INCLUDE)) \
		-o $@ -c
unit-all.o:
	@echo " \
#include <iostream>\n\
#include <cppunit/extensions/HelperMacros.h>\n\
#include <cppunit/ui/text/TestRunner.h>\n\
#include <cppunit/XmlOutputter.h>\n\
int main()\n\
{\n\
	CppUnit::TextUi::TestRunner runner;\n\
	runner.addTest( CppUnit::TestFactoryRegistry::getRegistry().makeTest() );\n\
	CppUnit::XmlOutputter *outputter = new CppUnit::XmlOutputter(\n\
			&runner.result(), std::cout);\n\
	runner.setOutputter(outputter);\n\
	return runner.run() ? 0 : 1;\n\
}" | \
	$(HOSTCXX) $< $(UNITTEST_CCFLAGS) -O0 -xc++ \
		$(patsubst %,-I%,$(UNITTEST_INCLUDE)) $(patsubst %,-I%,$($*_INCLUDE)) \
		-o $@ -c -

unit-all: unit-all.o $(patsubst %.cc,unit-%.o,$(UNITTEST_SOURCES))
	$(HOSTCXX) $^ \
		$(patsubst %,-L%,$(UNITTEST_LIBPATH)) $(patsubst %,-L%,$($*_LIBPATH)) \
		$(patsubst %,-l%,$(UNITTEST_LIBS)) $(patsubst %,-l%,$($*_LIBS)) \
		-o $@ $(UNITTEST_LDFLAGS)

INCLUDEDCLEANS := $(INCLUDEDCLEANS) unit-clean
unit-clean:
	-$(RM) unit-all unit-all.o
	-$(RM) $(patsubst %.cc,unit-%,$(UNITTEST_SOURCES))
	-$(RM) $(patsubst %.cc,unit-%.o,$(UNITTEST_SOURCES))
