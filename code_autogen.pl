#!/usr/bin/perl

open(F,"ctags -f - --fields=+S --excmd=number --c++-kinds=p --extra=+q *.h | grep -e '^[a-zA-Z]*::[a-zA-Z]*\\s.*signature:(' |");
while ($line=<F>) {
    chop $line;
    $line =~ /^([a-zA-Z]+)::([a-zA-Z]+).*signature:\((.*)\)$/;
    $struct = $1;
    $fname = $2;
    $sgn = $3;
    ($struct eq $fname) || next;
    print "found: $struct"."::$fname($sgn)\n";
}
