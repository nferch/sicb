/* Copyright 1990 by Carrick Sean Casey. */
/* For copying and distribution information, see the file COPYING. */

/* primitive to clear the history list */

#include <config.h>
#include "icb.h"
#include "externs.h"

c_hclear(TARGLIST)
TARGDEFS
{
	static char *usage = "usage: c_hclear";
	
	if (argc > 1)
		TRETURNERR(usage);

	histclear();
	return TCL_OK;
}
