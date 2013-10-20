/* 
 * strchr.c --
 *
 *	Source code for the "strchr" library routine.
 *
 * Copyright 1988 Regents of the University of California
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies.  The University of California
 * makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 */

#ifndef lint
static char rcsid[] = "$Header: /home/aurora1/mark/icb/tcl/RCS/strchr.c,v 4.6 1993/02/10 20:04:10 mark Exp $ SPRITE (Berkeley)";
#endif /* not lint */

#include <config.h>
#include <string.h>

/*
 *----------------------------------------------------------------------
 *
 * strchr --
 *
 *	Locate the first appearance of a character in a string.
 *
 * Results:
 *	The return value is the address of the first appearance
 *	in string of c.  If c doesn't appear in string then 0
 *	is returned.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

#ifndef HAVE_STRCHR
char *
strchr(string, c)
    register char *string;		/* String to search. */
    register char c;			/* Desired character. */
{
    while (1) {
	if (*string == c) {
	    return string;
	}
	if (*string++ == 0) {
	    return (char *) 0;
	}
    }
}
#endif
