/* Copyright (c) 1989 Carrick Sean Casey. All rights reserved. */

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <errno.h>
#include "select.h"
#include "ipcf.h"
#include "externs.h"

extern int errno;
int sock;
fd_set fdr;	
struct Cbuf buf;
int timeisup;

RETSIGTYPE
gotalarm()
{
	timeisup++;
}

clientserve()
{
	int ret;

	sock = port_fd; 

	/* make socket read and write errors detectable */
	signal(SIGPIPE, SIG_IGN);

	buf.new = 1;

	/* initialize fdset for select */
	FD_ZERO(&fdset);
	/* look at server fd */
	FD_SET(sock,&fdset); /**/
	/* look at tty fd */
	FD_SET(0,&fdset); /**/

	signal(SIGALRM, gotalarm);
	timeisup = 0;

	for (;;) {

		if (polldelay) {
			if (setitimer(ITIMER_REAL, polldelay, 0) < 0) {
				perror("client: setitimer failed");
				exit(-1);
			}
			timeisup = 0;
/*			sigpause(0); */
		}

		fdr = fdset;	/* should be a bcopy if larger than int */

		/* wait for input */

	/* NOTE - arg to FD_SET_SZ() should be highest fd we are listening to */
	/* for now we put 7 to limit it to one byte of looking */

		if ((ret = select(FD_SETSIZE, &fdr, 0, 0, polltimeout)) < 1) {
			if (ret == 0 || errno == EINTR) {
				c_didpoll();
				timeisup = 0;
			} else {
				perror("client: select");
				exit(-1);
			}
		}

		if (ret > 0)
			cdoinput();
		if (timeisup)
			c_didpoll();
	}
}

cdoinput()
{
	if (FD_ISSET(0, &fdr)) {
/*		printf("\nreading input!\n"); */
		c_userchar();
	}

	if (FD_ISSET(sock, &fdr)) {
/*		printf("\nreading socket!\n"); */
		switch(_readpacket(sock, &buf)) {
		case  1:
			/* complete packet */
			c_packet(buf.buf);
			break;
		case  0:
			/* incomplete packet */
/*			fprintf(stderr,"warn: client got incomplete\n");*/
			break;
		case -1:
			/* error */
			close(sock);
			c_lostcon();
			break;
		case -2:
			/* lost connection with server */
			close(sock);
			c_lostcon();
			break;
		}
	}
}

sendit(pkt)
char *pkt;
{
	switch(_sendpacket(sock, pkt)) {
	case  1:
		/* ok */
		break;
	case  0:
		/* incomplete packet */
		fprintf(stderr, "DEBUG: send blocked  - panic stop\n");
		exit(1);
		break;
	case -1:
		/* error */
		fprintf(stderr, "DEBUG: error in send - panic stop\n");
		exit(1);
		break;
	case -2:
		/* lost connection */
		fprintf(stderr, "server connection lost - bye\n");
		close(sock);
		exit(1);
	}
}

/* disconnect from the server */
closecon()
{
	close(sock);
	return(0);
}
