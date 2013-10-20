/* Copyright 1990 by Carrick Sean Casey. */
/* For copying and distribution information, see the file COPYING. */

/* primitive to change user info */

#include <config.h>
#include <string.h>
#include "icb.h"
#include "externs.h"

s_admin(TARGLIST)
TARGDEFS
{
	static char *usage = "usage: s_admin drop|shutdown|wall args";

	if (argc < 2 || !*argv[1])
		TRETURNERR(usage)

	if (strcmp(argv[1], "drop") == 0) {
		if (argc < 3)
			TRETURNERR("usage: s_admin drop id")
		sendcmd("drop", catargs(&argv[2]));
		return(TCL_OK);
	}

	else if (strcmp(argv[1], "shutdown") == 0) {
		if (argc < 3)
			TRETURNERR("usage: s_admin shutdown when exp")
		sendcmd("shutdown", catargs(&argv[2]));
		return(TCL_OK);
	}

	else if (strcmp(argv[1], "wall") == 0) {
		if (argc < 3)
			TRETURNERR("usage: s_admin wall text")
		sendcmd("wall", catargs(&argv[2]));
		return(TCL_OK);
	}

	else
		TRETURNERR(usage)
}
