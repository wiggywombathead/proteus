specification.pdf: specification.tex bibliography.bib
	pdflatex specification.tex
	bibtex specification.aux
	pdflatex specification.tex
	pdflatex specification.tex

clean: specification.pdf
	find . -type f -not -name 'Makefile' -not -name '*.md' -not -name '*.bib' -not -name '*.pdf' -not -name '*.tex' -delete
