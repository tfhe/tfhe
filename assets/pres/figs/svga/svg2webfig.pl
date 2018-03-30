#!/usr/bin/perl

$texmacros="macros.tex";
$svgfile="";
$unclean=0;


sub exec_cmd {
    $cmd = $_[0];
    print STDOUT "arg: $cmd\n";
    system($cmd);
}

$n = $#ARGV+1;
for ($i=0; $i<$n; $i+=1) {
    print STDERR "$i\n";
    if ($ARGV[$i] eq "-m") {$texmacros = $ARGV[$i+1]; $i+=1;}
    else { if ($ARGV[$i] eq "-u") {$unclean = 1; } }
}

$svgfile = $ARGV[$n-1];
$svgbn = $svgfile;
$svgbn =~ s/\.svg$//;

$lid='ull';
{
	exec_cmd("inkscape -z -P \"$svgbn-l$lid.eps\" -j -C "
	      ."--export-latex \"$svgfile\"");
	$bndbox=`grep '^%%DocumentMedia: ' \"$svgbn-l$lid.eps\"`;
	$bndbox=~ /DocumentMedia: [a-z0-9]* ([0-9.]*) ([0-9.]*)/;
	$paperw=$1;
	$paperh=$2;
	exec_cmd("sed -i 's/BoundingBox: .*/BoundingBox: 0 0 $paperw $paperh/' \"$svgbn-l$lid.eps\"");
	exec_cmd("epstopdf \"$svgbn-l$lid.eps\"");
	exec_cmd("sed -i 's/\\.eps}/.pdf}/' \"$svgbn-l$lid.eps_tex\"");

	#Parse the .eps_tex file to get width and height
	$picture='';
	$width=-1;
	$height=-1;
	$unit='xx';
	open H,"$svgbn-l$lid.eps_tex";
	while ($line = <H>) {
	    if ($line =~ /\\setlength\{\\unitlength\}\{([0-9.]*)([a-z]*)\}/) {
		$width = $1;
		$unit = $2;
		print STDERR "found width: $width $unit\n";
	    }
	    if ($line =~ /\\begin\{picture\}\(1,([0-9.]*)\)/) {
		$height = $1;
		$height = $1*$width;
		print STDERR "found height: $height $unit\n";
		$dump_picture=1;
	    }
	    if ($dump_picture) {
		$picture .= $line;
	    }
	    if ($line =~ /\\end\{picture\}/) {
		$dump_picture=0;
	    }
	}
	close H;
	
	open G,">$svgbn-f$lid.tex";
	print G <<EOF
\\documentclass{article}
\\pagestyle{empty}
\\usepackage{graphicx}
\\usepackage{color}
\\usepackage{eso-pic}
\\usepackage[paperwidth=$width$unit,paperheight=$height$unit]{geometry}
\\input{$texmacros}
\\AddToShipoutPicture{%
    \\setlength{\\unitlength}{$width$unit}
    $picture
}
\\begin{document}
~
\\end{document}
EOF
;
        close G;
	$cmd = "pdflatex \"$svgbn-f$lid.tex\" </dev/null 2>/dev/null >/dev/null";
	exec_cmd($cmd);
	exec_cmd($cmd);
	$cmd = "pdf2svg \"$svgbn-f$lid.pdf\" \"output/$svgbn-f$lid.svg\" </dev/null 2>/dev/null >/dev/null";
	exec_cmd($cmd);
	$cmd = "mv \"$svgbn-f$lid.pdf\" \"output/$svgbn-f$lid.pdf\" </dev/null 2>/dev/null >/dev/null";
	exec_cmd($cmd);
	
	#fait le ménage
	if (!$unclean) {
	exec_cmd("rm \"$svgbn-f$lid.tex\"");
	exec_cmd("rm \"$svgbn-f$lid.aux\"");
	exec_cmd("rm \"$svgbn-f$lid.log\"");
	exec_cmd("rm \"$svgbn-l$lid.eps_tex\"");
	exec_cmd("rm \"$svgbn-l$lid.eps\"");
	exec_cmd("rm \"$svgbn-l$lid.pdf\"");
    }
}

