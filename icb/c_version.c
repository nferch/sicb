/* Copyright 1994 by Mark J. Reed */
/* For copying and distribution information, see the file COPYING. */

/* primitive to display client version info */

#include <config.h>
#include "icb.h"
#include "externs.h"

c_version(TARGLIST)
TARGDEFS
{
	static char *usage = "usage: c_version";

	/* check the arg count */
	if (argc != 1)
		TRETURNERR(usage)

	putl(icb_version, PL_SCR);
	return(TCL_OK);
}
