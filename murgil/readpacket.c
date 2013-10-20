/* Copyright (c) 1988 Carrick Sean Casey. All rights reserved. */

/* read a packet of data into an Ibuf */

/* return -2 if read failed because user disconnected */
/* return -1 if read failed because of some error */
/* return  0 if packet is still incomplete */
/* return  1 if packet is complete */

#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include "select.h"
#include "ipcf.h"
#include "externs.h"

extern int errno;

_readpacket(user, p)
int user;
struct Cbuf *p;
{
	register int ret;

	if (p->new) {
		/* starting a new command */
		p->rptr = p->buf;
		/* read the length of the command packet */
		if ((ret = read(user, p->rptr, 1)) < 0)
			if ((errno == EWOULDBLOCK) || (errno == EAGAIN))
				return(0);
			else {
				fail = "_readpacket:read packet length";
				return(-1);
			}
		if (!ret)
			return(-2);
		p->size = p->remain = *(p->rptr);
		p->rptr++;
		p->new = 0;
	}

	/* read as much of the command as we can get */
	if ((ret = read(user, p->rptr, p->remain)) < 0)
		if ((errno == EWOULDBLOCK) || (errno == EAGAIN))
			return(0);
		else {
			fail = "_readpacket:read incomplete data";
			return(-1);
		}
	if (!ret)
		return(-2);

	/* advance read pointer */
	p->rptr += ret;

	/* see if we read the whole thing */
		if ((p->remain -= ret) == 0) {
			/* yes */
			p->new = 1;
			return(1);
		} else
			/* command still incomplete */
			return(0);
}
