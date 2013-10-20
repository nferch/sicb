/* Copyright (c) 1988 Carrick Sean Casey. All rights reserved. */

#include <sys/types.h>
#include <sys/socket.h>
#include "select.h"
#include "ipcf.h"
#include "externs.h"

/* turn off input polling for a user */

ignore(user)
int user;
{
	FD_SET(user, &ignorefds);
}


/* restore input polling for a user */

unignore(user)
int user;
{
	FD_CLR(user, &ignorefds);
}
