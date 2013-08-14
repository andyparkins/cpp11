# ----------------------------------------------------------------------------
# Version Control
#    $Author$
#      $Date$
#        $Id: ae47dd3388581bd7fde9b956b7b26e35c814cfb8 $
#
# Legal
#    Copyright 2013 FussyLogic Ltd
#
# ----------------------------------------------------------------------------

# parameters
SONAME           ?= template
TAGGEDSOVERSION  ?= 0.0.0
TAGGEDSOVERSIONL ?= 0
# all libraries
SONAME_LIST      := lib$(SONAME).a \
	lib$(SONAME).so \
	lib$(SONAME).so.$(TAGGEDSOVERSIONL) \
	lib$(SONAME).so.$(TAGGEDSOVERSION)

dynamic-library: $(SONAME_LIST)

# static library
lib$(SONAME).a: $(OBJS)
	$(RM) $@
	$(AR) rcs $@ $^

# dynamic library
lib$(SONAME).so.$(TAGGEDSOVERSION): $(OBJS)
	$(CXX) -shared $(LDFLAGS) \
		$(addprefix -L,$(foreach x,$(OBJS:.o=_LIBSPATH),$($x))) \
		$(addprefix -l,$(foreach x,$(OBJS:.o=_LIBS),$($x))) \
		-Wl,-soname,$@.$(TAGGEDSOVERSIONL) \
		-o $@ \
		$^

# dynamic library with versioned name
lib$(SONAME).so lib$(SONAME).so.$(TAGGEDSOVERSIONL): lib$(SONAME).so.$(TAGGEDSOVERSION)
	@echo "Making $@  $(TAGGEDSOVERSION)"
	ln -sf $< $@
	ln -sf $< $@.$(TAGGEDSOVERSIONL)


$(SONAME): $(OBJS)
	$(CXX) $(LDFLAGS) \
		$(addprefix -L,$(LIBPATH)) $(addprefix -L,$(foreach x,$(OBJS:.o=_LIBSPATH),$($x))) \
		$(addprefix -l,$(LIBS)) $(addprefix -l,$(foreach x,$(OBJS:.o=_LIBS),$($x))) \
		-o $@ \
		$^

INCLUDEDCLEANS := $(INCLUDEDCLEANS) dynamic-clean
dynamic-clean:
	$(RM) $(SONAME_LIST)

INCLUDEDPHONY := $(INCLUDEDPHONY) dynamic-library dynamic-clean
