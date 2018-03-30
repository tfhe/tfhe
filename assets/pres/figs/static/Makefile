PDFS=$(wildcard *.pdf)
SVGS=$(patsubst %.pdf,%.svg,${PDFS})

all: ${SVGS}

%.svg: %.pdf
	pdf2svg $< $@

