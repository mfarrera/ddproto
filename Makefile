TEX = pdflatex 
BIB = bibtex

PAPER = survey-memo
BIBFILE = ska.bib

all: $(PAPER).pdf 

spell::
	ispell *.tex

clean::
	rm -fv *.aux *.log *.bbl *.blg *.toc *.out *.lot *.lof $(PAPER).pdf 

$(PAPER).pdf: $(PAPER).tex $(PAPER).bbl 
	$(TEX) $(PAPER) 
	$(TEX) $(PAPER)

$(PAPER).bbl: $(PAPER).tex $(BIBFILE)
	$(TEX) $(PAPER).tex
	$(BIB) $(PAPER)

