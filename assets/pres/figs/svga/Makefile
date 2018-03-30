SVGS=$(wildcard *.svg)
WEB_ANIM=$(patsubst %.svg,output/%-f1.svg,${SVGS})

all: macros.tex output ${WEB_ANIM}


output/%-f1.svg: %.svg macros.tex
	perl svg2webanim.pl $<
	echo $@

output:
	mkdir output




