/* Copyright 1990 by Carrick Sean Casey. */
/* For copying and distribution information, see the file COPYING. */

/* primitive to change user info */

#include <config.h>
#include "icb.h"
#include "externs.h"

s_user(TARGLIST)
TARGDEFS
{
	static char *usage = "usage: s_user nick|echoback args";

	if (argc < 2 || !*argv[1])
		TRETURNERR(usage)

	if (strcmp(argv[1], "nick") == 0) {
		if (argc < 3)
			TRETURNERR("usage: s_user nick newnick")
		
		sendcmd("name", catargs(&argv[2]));
		return(TCL_OK);
	}

	else if (strcmp(argv[1], "echoback") == 0) {
		if (argc < 3)
			TRETURNERR("usage: s_user echoback on|off")
		sendcmd("echoback", catargs(&argv[2]));
		return(TCL_OK);
	}

	else
		TRETURNERR(usage)
}
