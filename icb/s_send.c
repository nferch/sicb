/* Copyright 1990 by Carrick Sean Casey. */
/* For copying and distribution information, see the file COPYING. */

/* primitive to send a line to a group */
/* this is included for Tcl purposes, but most text bypasses this */

#include <config.h>
#include <string.h>
#include "icb.h"
#include "externs.h"

s_send(TARGLIST)
TARGDEFS
{
	static char *usage = "usage: s_send text ...";

	if (argc < 2 || !*argv[1])
		TRETURNERR(usage)

	send_default(catargs(&argv[1]),0);
	return(TCL_OK);
}

send_default(text,echoflag)
char *text;
int echoflag;
{
if (gv.personalto && *gv.personalto)
	sendpersonal(gv.personalto, text, echoflag);
else
	csendopen(text);
}
