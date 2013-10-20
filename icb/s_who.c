/* Copyright 1990 by Carrick Sean Casey. */
/* For copying and distribution information, see the file COPYING. */

/* primitive to list users */

#include <config.h>
#include "icb.h"
#include "externs.h"

s_who(TARGLIST)
TARGDEFS
{
	static char *usage = "usage: s_who long|short [group]";

	/* check the arg count */
	if (argc < 2 || !*argv[1])
		TRETURNERR(usage)

	/* FIXME - right now, only long reports are is valid */
	if (strcmp(argv[1], "long"))
		TRETURNERR("s_who: only long who requests for now")

	if (argc == 3)
		sendcmd("w", argv[2]);
	else
		sendcmd("w", "");
	return(TCL_OK);
}
