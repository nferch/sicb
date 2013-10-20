/* Copyright (c) 1988 Carrick Sean Casey. All rights reserved. */

/* main program loop */
/* active clients, new connections, and perform asynch dungeon action */

#include <config.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <errno.h>
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif /* FCNTL */
#include "select.h"
#include "ipcf.h"
#include "externs.h"
 
extern int errno;
fd_set fdr;	
int timeisup;

RETSIGTYPE gotalarm();

serverserve()
{
	int ret;
	long loopcount = 0; /* used to break loop for periodic didpoll */

	/* look at fd where new clients connect */
	FD_ZERO(&fdset);
	FD_SET(port_fd, &fdset);

	if (port_fd > highestfd)
		highestfd = port_fd;

	signal(SIGALRM, gotalarm);
	timeisup = 0;

	for (;;) {
		if (polldelay) {
			if (setitimer(ITIMER_REAL, polldelay, 0) < 0) {
				perror("server: setitimer failed");
				exit(-1);
			}
			timeisup = 0;
/*			sigpause(0); */
		}


		/*fdr = fdset;	 should be a bcopy if larger than int */
		bcopy((char *) &fdset, (char *) &fdr,
		  (unsigned int) sizeof(fdset));

		/* wait for input */
		/* NULL should be changed to a time value that reflects */
		/* how much activity is going on */

		if ((ret = select(highestfd+1, &fdr, 0, 0, polltimeout)) < 1) {
			if (ret == 0 || errno == EINTR) {
				s_didpoll();
				timeisup = 0;
			} else {
				perror("select");
				exit(-1);
			}
		}

		if (ret > 0)
			sdoinput();
		if (timeisup || !(++loopcount % 10))
			s_didpoll();
	}
}

#if 0
sdoinput()
{
	int n;
	register int b;
	register int x, y;
	register unsigned long bits;

	/* examine set of file descriptors */
	for (b = 0, y = 0; b <= (howmany(highestfd, 32) - 1); b++, y += 32) {
	    if (!fdr.fds_bits[b])
		continue;
	    bits = fdr.fds_bits[b];
	    for (x = y; bits; x++, bits >>= 01)
		if (bits & 01)
			if (x == port_fd) {
				/* new connect on advertised socket */
				if ((n = _newconnect(x)) > 0)
					/* let server init the user */
					s_new_user(n);
			} else {
				/* fd is client input */
				switch(_readpacket(x, &cbufs[x])) {
				case  1:
					/* complete packet */
					s_packet(x, cbufs[x].buf);
					break;
				case  0:
					/* incomplete packet */
					break;
				case -1:
					/* error */
					break;
				case -2:
					/* close connection */
					disconnectuser(x);
					break;
				}
			}
	}
}
#endif

