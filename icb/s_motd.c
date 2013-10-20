/* Copyright 1990 by Carrick Sean Casey. */
/* For copying and distribution information, see the file COPYING. */

/* primitive to print the motd */

#include <config.h>
#include <string.h>
#include "icb.h"
#include "externs.h"

s_motd(TARGLIST)
TARGDEFS
{
	static char *usage = "usage: s_motd";

	/* check the arg count */
	if (argc != 1)
		TRETURNERR(usage)

	sendcmd("motd", "");
	return(TCL_OK);
}

