/* Copyright 1990 by Carrick Sean Casey. */
/* For copying and distribution information, see the file COPYING. */

/* primitive to exit icb */

#include <config.h>
#include "icb.h"
#include "externs.h"

c_quit(TARGLIST)
TARGDEFS
{
	static char *usage = "usage: c_quit";

	if (argc != 1) {
		/* if args are wrong, print usage but still quit */
		putl(usage, PL_SCR);
	}
	if (gv.verifyquit) {
		askquit();
		return(TCL_OK);
	}
		
	icbexit();
	/* NOTREACHED */
}
