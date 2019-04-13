SDIR = sections
SECTIONS = $(SDIR)/*.tex

report.pdf: $(SECTIONS) report.tex bibliography.bib
	pdflatex report.tex
	bibtex report.aux
	pdflatex report.tex
	pdflatex report.tex
	llpp report.pdf

count: $(SECTIONS) report.tex
	@../texcount.pl $(SECTIONS)

view: report.pdf
	llpp report.pdf

clean:
	rm -f report.pdf
