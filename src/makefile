CC=gcc
CPPC=gxx
RM=del
EXEC=bioscena.exe
CPPSOURCES:= $(wildcard *.cpp)
CSOURCES:=$(wildcard *.c)
OBJSCPP:=$(CPPSOURCES:.cpp=.o)
OBJSC:=$(CSOURCES:.c=.o)
CFLAGS= -g -Wall
LDLIBS=-lm 
LDFLAGS=-s --relax 

default: copirrait ${EXEC}

copirrait:
	@echo KKEPerians UNLTD generic makefile file 

dep: copirrait 
	@echo --- Generant dependencies
	$(CPPC) -MM $(CPPSOURCES) $(CSOURCES) -D_KKEP_BUSCANT_DEPENDENCIES > .depend

clean: copirrait 
	@echo --- Eliminant arxius intermedis
	$(RM) *.o 
	$(RM) ${EXEC}

include .depend

${EXEC}: $(OBJSCPP) $(OBJSC)
	@echo --- Linkant $@
	${CPPC} $(LDFLAGS) $(OBJSCPP) $(OBJSC) ${LDLIBS} -o ${EXEC} 
	buildnum

$(OBJSC) %.o: %.c
	@echo --- Compilant $<
	${CC} ${CFLAGS} -c $< -o $@


$(OBJSCPP) : %.o: %.cpp
	@echo --- Compilant $<
	${CPPC} ${CFLAGS} -c $< -o $@


