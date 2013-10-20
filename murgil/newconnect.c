/* Copyright (c) 1988 Carrick Sean Casey. All rights reserved. */

/* accept and initialize a new client connection */

/* returns: */
/* the resultant socket file descriptor */
/*  0 if operation would block */
/* -1 if operation failed */

#include <config.h>
#include <stdio.h>
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif /* FCNTL */
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include "select.h"
#include "ipcf.h"
#include "externs.h"
 
extern int errno;

_newconnect(s)
int s;
{
	int ns;
	int fromlen;
	int one = 1;
	struct sockaddr from;

	/* accept the connection */
	fromlen = sizeof(from);
	if ((ns = accept(s, (struct sockaddr *) &from, &fromlen)) < 0)
		if ((errno == EWOULDBLOCK) || (errno == EAGAIN))
			return(0);
		else {
			fail = "new_client:accept";
			return(-1);
		}

	/* force occasional connection check */
	if (setsockopt(ns, SOL_SOCKET, SO_KEEPALIVE,
	  (char *)&one, sizeof(one)) < 0) {
		fail = "newclient:setsockopt (keepalive)";
		return(-1);
	}

	/* don't allow broken connections to linger */
/*
	if (setsockopt(ns, SOL_SOCKET, SO_LINGER,
	  (char *)&zero, sizeof(zero)) < 0) {
		fail = "newclient:setsockopt (dontlinger)";
		return(-1);
	}
*/

	/* make the socket non-blocking */
#if !defined(SYSV) && !defined(linux)
	if (fcntl(ns, F_SETFL, FNDELAY) < 0) {
#else /* SYSV */
	if (fcntl(ns, F_SETFL, O_NDELAY) < 0) {
#endif /* SYSV */
		fail = "new_client:fcntl";
		return(-1);
	}

	/* enter the user's fd into the active set */
	FD_SET(ns, &fdset);

	if (ns > highestfd)
		highestfd = ns;

	/* first command is a "new" command */
	cbufs[ns].new = 1;

	return(ns);
}
