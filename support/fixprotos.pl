# Copyright (c) 1990 by Carrick Sean Casey.
# For copying and distribution information, see the file COPYING.

# Usage: perl defmake.pl < def_procs > defprocs.c

print "/*\
	THIS IS AN AUTOMATICALLY GENERATED FILE\
\
	If you edit this file, your changes may be lost later.\
*/\n\n";

while (<>) {
	chop;
	s/(.)char(\s*[A-Za-z])/$1int$2/g;
	s/(.)short(\s*[A-Za-z])/$1int$2/g;
	s/ va_alist\)/ va_alist,...\)/g;
	print "$_\n";
}
