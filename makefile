RM=del
MAINTEX:=bioscena.tex
POSTSCRIPT=$(MAINTEX:.tex=.ps)
DVI:=$(MAINTEX:.tex=.dvi)
PDF:=$(MAINTEX:.tex=.pdf)
HTML:=$(MAINTEX:.tex=.html)
TEXSOURCES:= $(wildcard *.tex)
BIBSOURCES:= $(wildcard *.bib)
JPGSOURCES:= $(wildcard *.jpg)
BOUNDINGBOXES:= $(JPGSOURCES:.jpg *.bb)

all: copirrait ${POSTSCRIPT} ${PDF}  $(HTML)

default: copirrait ${POSTSCRIPT}

pdf: copirrait ${PDF}

ps: copirrait ${POSTSCRIPT}

html: copirrait $(HTML)

copirrait:
	@echo KKEPerians UNLTD LaTeX makefile file 
	buildnum

clean: copirrait 
	@echo --- Eliminant arxius intermedis
	$(RM) *.dvi
	$(RM) *.aux
	$(RM) *.toc
	$(RM) ${POSTSCRIPT}

${POSTSCRIPT}: $(DVI)
	@echo --- Generant postscript $@
	dvips $(DVI) -z -o $(POSTSCRIPT)

$(BOUNDINGBOXES) *.bb: *.jpg
	@echo --- Generant Boundin Box per a $<
	ebb $<

$(DVI): $(TEXSOURCES) $(BOUNDINGBOXES)
	@echo --- Compilant $<
	latex $(MAINTEX)
	latex $(MAINTEX)


$(PDF): $(TEXSOURCES)
	@echo --- Compilant $<
	pdflatex $(MAINTEX)
	pdflatex $(MAINTEX)

$(HTML): $(TEXSOURCES)
	tth -L <$(MAINTEX) >$@
