# Opciones
# all ps pdf html
RM=rm
PAGER=less
MAIN:=bioscena
MAINTEX:=$(MAIN).tex
PS=$(MAIN).ps
DVI:=$(MAIN).dvi
PDF:=$(MAIN).pdf
HTML:=$(MAIN).html
AUX:=$(MAIN).aux
IDX:=$(MAIN).idx
TOC:=$(MAIN).toc
IND:=$(MAIN).ind
BBL:=$(MAIN).bbl
TEXSOURCES:= $(wildcard *.tex)
BIBSOURCES:= $(wildcard *.bib)
JPGSOURCES:= $(wildcard *.jpg)
GIFSOURCES:= $(wildcard *.gif)
PNGSOURCES:= $(wildcard *.png)
BOUNDINGBOXES:= $(JPGSOURCES:.jpg=.bb)
INTERMEDIATES:= $(AUX) $(IDX)  $(IND) $(BBL) $(TOC)
LOGS:=$(wildcard *.out) $(wildcard *.log) $(wildcard *.blg) $(wildcard *.ilg)
FINALS:= $(DVI) $(PDF) $(HTML) $(PS)
#TEXOPTS:=--src-specials --c-style-errors
TEXOPTS:=

default: copirrait
	pdflatex $(TEXOPTS) $(MAIN)
	bibtex $(MAIN)
	makeindex $(MAIN)
	pdflatex $(TEXOPTS) $(MAIN)

ps: copirrait ${POSTSCRIPT}
pre: copirrait $(AUX) $(BBL) $(IDX)
all: copirrait ${POSTSCRIPT} ${PDF}  $(HTML)
dvi: copirrait ${DVI}
pdf: copirrait ${PDF}
html: copirrait $(HTML)

clean: copirrait 
	@echo
	@echo --- Eliminant arxius intermitjos ----------------------
	$(RM) $(INTERMEDIATES) $(FINALS) $(LOGS)

cleannoout: copirrait 
	@echo
	@echo --- Eliminant arxius intermitjos ----------------------
	$(RM) $(INTERMEDIATES) $(LOGS)

todo: copirrait 
	@echo
	@echo --- Llistant feines pendents --------------------------
	grep -n TODO $(TEXSOURCES) $(BIBSOURCES) | $(PAGER)

copirrait:
	@echo --- KKEPerians UNLTD LaTeX makefile file --------------
	buildnum

$(PDF): $(TEXSOURCES)
	@echo
	@echo --- Generant Sortida PDF $@ ---------------------------
	pdflatex $(TEXOPTS) $(MAINTEX)

$(BOUNDINGBOXES): $(JPGSOURCES) 
	@echo
	@echo --- Bounding box $(JPGSOURCE) -------------------------
	ebb $<

$(AUX): $(TEXSOURCES) $(AUX) $(TOC) $(IND) $(BBL) $(BIBSOURCES) $(BOUNDINGBOXES)
	@echo
	@echo --- Precompilant $(MAINTEX) ---------------------------
	latex $(TEXOPTS) $(MAINTEX)
	latex $(TEXOPTS) $(MAINTEX)

$(TOC): $(TEXSOURCES)

$(IDX): $(TEXSOURCES)

$(IND): $(IDX)
	@echo
	@echo --- Generant Index IDX $@ -----------------------------
	makeindex $< -o $@ 

$(BBL): $(BIBSOURCES) $(AUX)
	bibtex $(MAIN)

$(DVI): $(TEXSOURCES)
	@echo
	@echo --- Generant Sortida DVI $@ ---------------------------
	latex $(TEXOPTS) $(MAINTEX)

${POSTSCRIPT}: $(DVI) 
	@echo
	@echo --- Generant Sortida Postscript $@ --------------------
	dvips $(DVI) -z -o $(POSTSCRIPT)

$(HTML): $(TEXSOURCES)
	@echo
	@echo --- Generant Sortida HTML $@ --------------------------
	tth -L <$(MAINTEX) >$@

