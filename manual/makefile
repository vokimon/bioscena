RM=del
POSTSCRIPT=bioscena.ps
DVI:=$(POSTSCRIPT:.ps=.dvi)
MAINTEX:=$(POSTSCRIPT:.ps=.tex)
TEXSOURCES:= $(wildcard *.tex)

default: copirrait ${POSTSCRIPT}

copirrait:
	@echo KKEPerians UNLTD LaTeX makefile file 

clean: copirrait 
	@echo --- Eliminant arxius intermedis
	$(RM) *.dvi
	$(RM) *.aux
	$(RM) *.toc
	$(RM) ${POSTSCRIPT}

${POSTSCRIPT}: $(DVI)
	@echo --- Generant postscript $@
	dvips $(DVI) -o $(POSTSCRIPT)
	buildnum

$(DVI): $(TEXSOURCES)
	@echo --- Compilant $<
	latex $(MAINTEX)
	latex $(MAINTEX)


