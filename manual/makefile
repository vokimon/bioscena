# Opciones
# all ps pdf html
RM=rm
MAINBASE:=bioscena
MAINTEX:=$(MAINBASE).tex
POSTSCRIPT=$(MAINBASE).ps
DVI:=$(MAINBASE).dvi
PDF:=$(MAINBASE).pdf
HTML:=$(MAINBASE).html
AUX:=$(MAINBASE).aux
IDX:=$(MAINBASE).idx
TOC:=$(MAINBASE).toc
IND:=$(MAINBASE).ind
BBL:=$(MAINTEX:.tex=.bbl)
TEXSOURCES:= $(wildcard *.tex)
BIBSOURCES:= $(wildcard *.bib)
JPGSOURCES:= $(wildcard *.jpg)
GIFSOURCES:= $(wildcard *.gif)
BOUNDINGBOXES:= $(JPGSOURCES:.jpg=.bb)
INTERMEDIATES:= $(AUX) $(IDX)  $(IND) $(BBL) $(TOC)
LOGS:=$(wildcard *.out) $(wildcard *.log) $(wildcard *.blg) $(wildcard *.ilg) head.tmp body.tmp
FINALS:= $(DVI) $(PDF) $(HTML) $(PS)
#TEXOPTS:=--src-specials --c-style-errors
TEXOPTS:=

default: copirrait $(AUX) $(BBL) $(IDX) ${POSTSCRIPT} 
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
	grep -n TODO $(TEXSOURCES) $(BIBSOURCES) | more

copirrait:
	@echo --- KKEPerians UNLTD LaTeX makefile file --------------
	buildnum

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
	bibtex $(MAINBASE)

$(DVI): $(TEXSOURCES)
	@echo
	@echo --- Generant Sortida DVI $@ ---------------------------
	latex $(TEXOPTS) $(MAINTEX)

${POSTSCRIPT}: $(DVI) 
	@echo
	@echo --- Generant Sortida Postscript $@ --------------------
	dvips $(DVI) -z -o $(POSTSCRIPT)

$(PDF): $(TEXSOURCES)
	@echo
	@echo --- Generant Sortida PDF $@ ---------------------------
	pdflatex $(TEXOPTS) $(MAINTEX)

$(HTML): $(TEXSOURCES)
	@echo
	@echo --- Generant Sortida HTML $@ --------------------------
	tth -L <$(MAINTEX) >$@

