/* Copyright 1990 by Carrick Sean Casey. */
/* For copying and distribution information, see the file COPYING. */

/* primitive to send a line to a group */
/* this is included for Tcl purposes, but most text bypasses this */

#include <config.h>
#include <string.h>
#include "icb.h"
#include "externs.h"

s_open(TARGLIST)
TARGDEFS
{
	static char *usage = "usage: s_open text ...";

	if (argc < 2 || !argv[1][0])
		TRETURNERR(usage)

	csendopen(catargs(&argv[1]));
	return(TCL_OK);
}
