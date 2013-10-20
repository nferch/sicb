/* Copyright (c) 1990 by Carrick Sean Casey. */
/* For copying and distribution information, see the file COPYING. */

/*

This file contains all the IPCF code for the client. Technically
speaking, the client is an event driven program. It is a bunch
of modules called up by an IPCF core. This core tells forum when the
user types a line, when the server hands it some data, when the
connection is lost, and optionally, when a certain amount of time
expires. Forum reacts to this and then returns control to the IPCF
core.

*/

#include <config.h>
#include "icb.h"
#include "externs.h"

/* called by the ipcf lib whenever an incoming packet arrives */
/* pkt is a pointer to a string */
/* the first byte is the packet length */
/* the remaining bytes are the contents of the packet */

void
c_packet(pkt)
char *pkt;
{
	/* go figure out what kind of packet it is */
	dispatch(++pkt);
}


/* called by ipcf lib whenever the server and tty are polled */
/* can be used for periodic functions */

void
c_didpoll()
{
	/* do nothing for now */
}


/* called by ipcf lib whenever the connection with the server is lost */

void
c_lostcon()
{
	/* report the problem, cleanup and exit */
	putl("Lost connection with the server. Quitting...", PL_ALL);
	icbexit();
}


/* called whenever the user types something */

void
c_userchar()
{
	int ret;
	static char *s;	/* user input string */

	/* user typed one or more characters - gather a line of input */
	s=getline(NULL, 1);

	if (!s) /* EOF */
	{ 	
		askquit();
		return;
	}

	if (!*s) /* empty line */
	{
		if (gv.cute) 
				saysomething();
	}
	else
	{
		/* parse the line */
		parse(s);
	}
}
