/* Copyright 1993 by Mark J. Reed */
/* For copying and distribution information, see the file COPYING. */

/* primitive to prompt for password and send registration command to server */

#include <config.h>
#include "icb.h"
#include "externs.h"

s_register(TARGLIST)
TARGDEFS
{
	static char *usage = "usage: s_register [password]";
	char *password;
	char message[256];

	if (argc > 2 || argc < 1)
		TRETURNERR(usage)

	if (argc == 2)
		regnick((char *)0);
	else
		regnick(argv[1]);
	return(TCL_OK);
}
