######################################################################
# Generic C++ Project Makefile v3.0a
# Copyright (c) 1998-2001 Vokimon/KKEPerian UNLTD
# Copyright (c) 1998-2001 Helly/Patata Productions
######################################################################

# Change the following options as suited to your project
	# The name of the executable file
	# Flags for the linker
PROGRAM=bioscena
EXECSUFFIX=
LDFLAGS= --relax -s
	# Library flags for the linker
LDLIBS=-lm -lcppunit
	# Flags for the C compiler
CFLAGS=-Wall -pedantic -O5 -march=i686 # -g -fexceptions

# Change the following options as suited to your system
	# The C compiler
CC=gcc
	# The C++ compiler
CXX=g++
	# Command used to delete files
RM=rm
	# The build counter program (empty if none)
BUILDCOUNTER=./buildnum

CPPEXTENSIONS=cxx
HPPEXTENSIONS=hxx hpp h
OBJDIR=../Obj/
DEPDIR=../Dep/
EXCLUDED=Topologies/Topology.Test.cxx
EXCLUDED_SOURCES=
EXCLUDED_HEADERS=
DIRSNOTTOHEADER=


#####################################################################
# Non user configuration

EXEC=$(PROGRAM)$(EXECSUFFIX)
DIRS=$(shell find * -type d -not -name CVS)
FILES=$(filter-out $(EXCLUDED), $(shell find * -not -type d | grep -v CVS))
CPPSOURCES=$(filter-out $(EXCLUDED_SOURCES), $(foreach ext, $(CPPEXTENSIONS), $(filter %.$(ext), $(FILES))))
CPPHEADERS=$(filter-out $(EXCLUDED_HEADERS),   $(foreach ext, $(HPPEXTENSIONS), $(filter %.$(ext), $(FILES))))
export CPLUS_INCLUDE_PATH+= $(subst $(empty) $(empty),:,$(filter-out $(DIRSNOTTOHEADER), $(DIRS)))
$(shell touch .depend)

# Those variables rarely need changes (pe. When you use .cxx extension for c++)
#CPPSOURCES:= $(wildcard *.cxx)
#CSOURCES:=$(wildcard *.c)
OBJSCPP:=$(CPPSOURCES:.cxx=.o)
OBJSC:=$(CSOURCES:.c=.o)

.PHONY: default dep ref clean debug-makefile

default: debug-makefile copirrait ${EXEC}

debug-makefile:
	@echo FILES: $(FILES)
	@echo CPPSOURCES: $(CPPSOURCES)
	@echo CPLUS_INCLUDE_PATH: $(CPLUS_INCLUDE_PATH)

copirrait:
	@echo KKEPerians UNLTD Generic C++ Project Makefile v3.0
	@echo --- Incrementant el nombre de builds
	@ ${BUILDCOUNTER}

tarball: clean
	tar cvfz ../$(PROGRAM)-`date +"%Y%m%d"`.tar.gz .

dep: copirrait .depend
	@echo --- Generant dependencies
	$(CXX) -MM $(CPPSOURCES) $(CSOURCES) -D_KKEP_BUSCANT_DEPENDENCIES > .depend

.depend: $(CPPSOURCES) $(CSOURCES) $(CPPHEADERS)

ref: $(CPPSOURCES)
	cd ../Reference; doxygen

clean: copirrait 
	@echo --- Eliminant arxius intermedis
	$(RM) ${EXEC} $(addprefix $(OBJDIR),$(OBJSCPP) $(OBJSC))

include .depend

${EXEC}: $(addprefix $(OBJDIR),$(OBJSCPP) $(OBJSC))
	@echo --- Linkant $@
	${CXX} $(LDFLAGS) $(addprefix $(OBJDIR),$(OBJSCPP) $(OBJSC)) ${LDLIBS} -o ${EXEC}

$(OBJDIR)%.o: %.c $(basename,%.o)
	@echo --- Compilant $<
	@mkdir -p `dirname $@`
	${CC} ${CFLAGS} -c $< -o $@

$(OBJDIR)%.o: %.cxx $(basename,%.o)
	@echo --- Compilant $<
	@mkdir -p `dirname $@`
	${CXX} ${CFLAGS} -c $< -o $@


