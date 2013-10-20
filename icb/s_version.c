/* Copyright 1990 by Carrick Sean Casey. */
/* For copying and distribution information, see the file COPYING. */

/* primitive to request ICB version info */

#include <config.h>
#include <string.h>
#include "icb.h"
#include "externs.h"

s_version(TARGLIST)
TARGDEFS
{
	static char *usage = "usage: s_version";

	/* check the arg count */
	if (argc != 1)
		TRETURNERR(usage)

	sendcmd("v", "");
	return(TCL_OK);
}
