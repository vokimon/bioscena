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
CPPC=g++
	# Command used to delete files
RM=rm
	# The build counter program (empty if none)
BUILDCOUNTER=./buildnum

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
	$(CPPC) -M $(CPPSOURCES) $(CSOURCES) -D_KKEP_BUSCANT_DEPENDENCIES > .depend

.depend:
	touch .depend

clean: copirrait 
	@echo --- Eliminant arxius intermedis
	$(RM) *.o ${EXEC}

include .depend

${EXEC}: $(OBJSCPP) $(OBJSC)
	@echo --- Linkant $@
	${CPPC} $(LDFLAGS) $(OBJSCPP) $(OBJSC) ${LDLIBS} -o ${EXEC}

$(OBJSC) : %.o: %.c
	@echo --- Compilant $<
	${CC} ${CFLAGS} -c $< -o $@


$(OBJSCPP) : %.o: %.cpp
	@echo --- Compilant $<
	${CPPC} ${CFLAGS} -c $< -o $@


