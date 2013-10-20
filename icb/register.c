/* Copyright 1993 by Mark J. Reed */
/* For copying and distribution information, see the file COPYING. */

/* primitive to prompt for password and send registration command to server */

#include <config.h>
#include "icb.h"
#include "externs.h"

void
regnick(password)
char *password;
{
	char message[256];

	if (!password)
		password = (char *)getpass("Password:");
	
	sprintf(message,"p %s", password);
	sendpersonal("server",message,-1);
}

