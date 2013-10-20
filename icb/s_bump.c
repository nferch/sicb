/* Copyright 1994 by Mark J. Reed */
/* For copying and distribution information, see the file COPYING. */

/* primitive to bump a user with your registered nickname -
   will prompt for the password (VMS server) */

#include <config.h>
#include "icb.h"
#include "externs.h"

s_bump(TARGLIST)
TARGDEFS
{
	static char *usage = "usage: s_bump nickname [password]";
	char *nick, *password;
	char message[256];

	if (argc > 3 || argc < 2)
		TRETURNERR(usage)

	nick = argv[1];

	if (argc > 2)
		password = argv[2];
	else
		password = (char *) getpass ("Enter password:");

	sprintf(message,"bump %s %s", nick, password);
	sendpersonal("server",message,-1);
	return(TCL_OK);
}
