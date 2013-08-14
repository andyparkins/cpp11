# ----------------------------------------------------------------------------
# Version Control
#    $Author: andyp $
#      $Date$
#        $Id: fa758eedff30146d960b43c02b2c99f921b368f9 $
#
# Legal
#    Copyright 2013  FussyLogic Ltd
#
# ----------------------------------------------------------------------------

VERSIONSTRINGRAW    ?= $(shell git describe 2> /dev/null)

# Fallback to using the ISO date if there's no git
ifeq ($(VERSIONSTRINGRAW),)
    VERSIONSTRINGRAW    := $(shell date -u +%Y%m%d%H)
    TAGGEDVERSION       := $(VERSIONSTRINGRAW)
else
    TAGGEDVERSION       := $(shell git describe --abbrev=0 2> /dev/null)
endif

VERSIONSTRING       := -$(VERSIONSTRINGRAW)
VERSIONSTRINGPREFIX := $(VERSIONSTRINGRAW)-
TAGGEDSOVERSION     := $(shell echo "$(TAGGEDVERSION)" | sed 's/^[a-zA-Z]*//g')
TAGGEDSOVERSIONL    := $(shell echo "$(TAGGEDSOVERSION)" | sed 's/\.[0-9.]*//g')



auto_revision_warning:
	@echo "****** WARNING: You have included revision.mak before your own recipes"

# ---
# We want to make version-holding targets that are only rebuilt when the
# version changed, this prevents builds and links of the rest of the project
# when nothing has really changed.
#
# Unfortunately make only triggers rebuilds based on file timestamps, so we
# need a method of converting a change of git-describe output into a change of
# a file.  We do this as follows:
#
#  - The targets should depend on `auto_revision.stamp`.
#  - `auto-revision.stamp` in turn depends on `auto-revision.stamp.tmp`
#  - `auto_revision.stamp.tmp` is always rebuilt with the latest version
#    string.
#  - Building `auto-revision.stamp` then compares its current content
#    with the `.tmp` file, and if they are different, copies the `.tmp`
#    to the `.stamp`.
#

# stamp files -- should be added to .gitignore
auto_revision.stamp.tmp:
	@echo "$(VERSIONSTRINGRAW)" > $@
auto_revision.stamp: auto_revision.stamp.tmp
	-@([ ! -e "$@" ] || ! cmp -s "$@" "$<") && "cp" -v "$<" "$@"
	-@rm -f $<


# C revision module
auto_revision.h:
	@echo "/* $@ */" > $@
	@echo "#ifdef __cplusplus" >> $@
	@echo "extern \"C\" {" >> $@
	@echo "#endif" >> $@
	@echo "extern const char *VERSIONSTRINGRAW;" >> $@
	@echo "extern const char *TAGGEDVERSION;" >> $@
	@echo "extern const char *VERSIONSTRING;" >> $@
	@echo "extern const char *VERSIONSTRINGPREFIX;" >> $@
	@echo "#ifdef __cplusplus" >> $@
	@echo "}" >> $@
	@echo "#endif" >> $@
auto_revision.c: auto_revision.h auto_revision.stamp
	@echo "/* $@ */" > $@
	@echo "#include \"auto_revision.h\"" >> $@
	@echo "const char *VERSIONSTRINGRAW    = \"$(VERSIONSTRINGRAW)\";" >> $@
	@echo "const char *TAGGEDVERSION       = \"$(TAGGEDVERSION)\";" >> $@
	@echo "const char *VERSIONSTRING       = \"$(VERSIONSTRING)\";" >> $@
	@echo "const char *VERSIONSTRINGPREFIX = \"$(VERSIONSTRINGPREFIX)\";" >> $@
auto_revision.o: auto_revision.c auto_revision.h
	$(CC) -o $@ -c $<


# Latex revision module
auto_revision.tex:
	@echo "% $@" > $@
	@echo "\\\\newcommand{\\\\VERSIONSTRINGRAW}{$(VERSIONSTRINGRAW)}" >> $@
	@echo "\\\\newcommand{\\\\TAGGEDVERSION}{$(TAGGEDVERSION)}" >> $@
	@echo "\\\\newcommand{\\\\VERSIONSTRING}{$(VERSIONSTRING)}" >> $@
	@echo "\\\\newcommand{\\\\VERSIONSTRINGPREFIX}{$(VERSIONSTRINGPREFIX)}" >> $@


INCLUDEDCLEANS := $(INCLUDEDCLEANS) revision-clean
revision-clean:
	-$(RM) auto_revision.stamp.tmp auto_revision.stamp
	-$(RM) auto_revision.h auto_revision.c auto_revision.o
	-$(RM) auto_revision.tex

INCLUDEDPHONY := $(INCLUDEDPHONY) revision-clean
