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
BOUNDINGBOXES:= $(JPGSOURCES:.jpg *.bb)
INTERMEDIATES:= $(AUX) $(IDX)  $(IND) $(BBL) $(TOC)
LOGS:=$(wildcard *.out) $(wildcard *.log) $(wildcard *.blg) $(wildcard *.ilg) head.tmp body.tmp
FINALS:= $(DVI) $(PDF) $(HTML) $(PS)
#TEXOPTS:=--src-specials --c-style-errors
TEXOPTS:=

default: copirrait ${POSTSCRIPT}

all: copirrait ${POSTSCRIPT} ${PDF}  $(HTML)

pdf: copirrait ${PDF}

ps: copirrait ${POSTSCRIPT}

html: copirrait $(HTML)

copirrait:
	@echo KKEPerians UNLTD LaTeX makefile file ------------------
	buildnum

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
	@echo --- Llistant feines pendents ----------------------
	grep -n TODO $(TEXSOURCES) $(BIBSOURCES) | more


$(AUX): $(TEXSOURCES) $(BIBSOURCES)
	@echo
	@echo --- Precompilant $(MAINTEX) -----------------------------------
	latex $(TEXOPTS) $(MAINTEX)

$(TOC): $(TEXSOURCES)

$(IDX): $(TEXSOURCES)

$(IND): $(IDX)
	@echo
	@echo --- Generant Index IDX $@ -----------------------------
	makeindex $< -o $@ 

$(DVI): $(AUX) $(TOC) $(IND) $(BBL)
	@echo
	@echo --- Generant Sortida DVI $@ ---------------------------
	latex $(TEXOPTS) $(MAINTEX)
	latex $(TEXOPTS) $(MAINTEX)

$(BBL): $(BIBSOURCES) $(AUX)
	bibtex $(MAINBASE)

${POSTSCRIPT}: $(DVI)
	@echo
	@echo --- Generant Sortida Postscript $@ --------------------
	dvips $(DVI) -z -o $(POSTSCRIPT)

$(PDF): $(AUX) $(TOC) $(IDX)
	@echo
	@echo --- Generant Sortida PDF $@ ---------------------------
	pdflatex $(TEXOPTS) $(MAINTEX)

$(HTML): $(AUX) $(TOC) $(IDX)
	@echo
	@echo --- Generant Sortida HTML $@ --------------------------
	tth -L <$(MAINTEX) >$@

