/* Copyright (c) 1988 by Carrick Sean Casey. All rights reserved. */

/* shutdown a connection and take it out of the input loop */

#include <sys/types.h>
#include "select.h"
#include "ipcf.h"
#include "externs.h"

disconnectuser(user)
int user;
{
	int i;

	close(user);
	FD_CLR(user,&fdset);
	if (user == highestfd) {
		for (i = FD_SETSIZE-1; i > 0 && !FD_ISSET(i, &fdset); i--);
		highestfd = i;
	}
	/* let main program know user went bye bye */
	s_lost_user(user);
}
