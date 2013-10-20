/* Copyright (c) 1988 Carrick Sean Casey. All rights reserved. */

/* send a packet along a socket file descriptor */

#include <stdio.h>
#include <errno.h>

extern int errno;

_sendpacket(s, pkt)
int s;
char *pkt;
{
	int ret;
	int totlength;

	/* set up a sample packet for testing purposes */
	totlength = (unsigned char)*pkt + 1;

	if ((ret = send(s, pkt, totlength, 0)) < 0) {
		if ((errno == EWOULDBLOCK) || (errno == EAGAIN))
			return(0);
		else if (errno == EPIPE)
			return(-2);
		else {
			/* DEBUG temorary fix */
			return(-2);
#ifdef NEVERDEFFED
			perror("DEBUG: _sendpacket: error in send");
			kill(getpid(), 3); /* BOOM! debug */
			sleep(20000);
			return(-1);
#endif
		}
	}
	if (ret != totlength)
		return(-1);
	return(1);
}
