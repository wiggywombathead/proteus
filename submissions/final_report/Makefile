SDIR = sections
SECTIONS = $(SDIR)/*.tex

report.pdf: $(SECTIONS) report.tex bibliography.bib
	pdflatex report.tex
	bibtex report.aux
	pdflatex report.tex
	pdflatex report.tex

clean:
	rm -f report.pdf
