RM=del
POSTSCRIPT=bioscena.ps
DVI:=$(POSTSCRIPT:.ps=.dvi)
PDF:=$(POSTSCRIPT:.ps=.pdf)
MAINTEX:=$(POSTSCRIPT:.ps=.tex)
TEXSOURCES:= $(wildcard *.tex)

default: copirrait ${POSTSCRIPT}

pdf: copirrait ${PDF}

ps: copirrait ${POSTSCRIPT}

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
	dvips $(DVI) -z -o $(POSTSCRIPT)
	buildnum

$(DVI): $(TEXSOURCES)
	@echo --- Compilant $<
	latex $(MAINTEX)
	latex $(MAINTEX)


$(PDF): $(TEXSOURCES)
	@echo --- Compilant $<
	pdflatex $(MAINTEX)
	pdflatex $(MAINTEX)
	buildnum


