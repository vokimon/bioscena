######################################################################
# Generic C++ Project Makefile v2.1
# Copyright (c) 1998-2001 Vokimon/KKEPerian UNLTD
# Copyright (c) 1998-2001 Helly/Patata Productions
######################################################################

# Change the following options as suited to your project
	# The name of the executable file
EXEC=./bioscena.exe
	# Flags for the linker
LDFLAGS= --relax -s
	# Library flags for the linker
LDLIBS=-lm
	# Flags for the C compiler
CFLAGS=-Wall -pedantic -Os -march=i686# -g -fexceptions

# Change the following options as suited to your system
	# The C compiler
CC=gcc
	# The C++ compiler
CXX=g++
	# Command used to delete files
RM=rm
	# The build counter program (empty if none)
BUILDCOUNTER=./buildnum

OBJDIR=../Obj/

# Those variables rarely need changes (pe. When you use .cxx extension for c++)
CPPSOURCES:= $(wildcard *.cpp)
CSOURCES:=$(wildcard *.c)
OBJSCPP:=$(CPPSOURCES:.cpp=.o)
OBJSC:=$(CSOURCES:.c=.o)

default: copirrait ${EXEC}

copirrait:
	@echo KKEPerians UNLTD Generic C++ Project Makefile v2.1
	@echo --- Incrementant el nombre de builds
	@ ${BUILDCOUNTER}

dep: copirrait .depend
	@echo --- Generant dependencies
	$(CXX) -MM $(CPPSOURCES) $(CSOURCES) -D_KKEP_BUSCANT_DEPENDENCIES > .depend

.depend:
	touch .depend

clean: copirrait 
	@echo --- Eliminant arxius intermedis
	$(RM) $(OBJDIR)*.o ${EXEC}

include .depend

${EXEC}: $(addprefix $(OBJDIR),$(OBJSCPP) $(OBJSC))
	@echo --- Linkant $@
	${CXX} $(LDFLAGS) $(addprefix $(OBJDIR),$(OBJSCPP) $(OBJSC)) ${LDLIBS} -o ${EXEC}

$(OBJDIR)%.o: %.c
	@echo --- Compilant $<
	@mkdir -p `dirname $@`
	${CC} ${CFLAGS} -c $< -o $@


$(OBJDIR)%.o: %.cpp
	@echo --- Compilant $<
	@mkdir -p `dirname $@`
	${CXX} ${CFLAGS} -c $< -o $@


