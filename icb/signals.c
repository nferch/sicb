/* Copyright (c) 1990 by Carrick Sean Casey. */
/* For copying and distribution information, see the file COPYING. */

/* signal handling routines */

#include <config.h>
#include "icb.h"
#include "externs.h"
#include <signal.h>

#ifdef HAVE_READLINE_H
#include <readline.h>
#endif

#ifdef HAVE_READLINE_READLINE_H
#include <readline/readline.h>
#endif

/* Note that some of these routines set "continue" to 1. Continue is used */
/* by a few other icb routines to see if they've been interrupted */

/* The signal stuff is not portable, and should be in unix.c. */

#define	mask(s)	(1 << ((s)-1))

/* set up the signal trappers */

trapsignals()
{
/*	sighandler askquit(), suspend(), icbexit(); */

	/* exit on a hangup or terminate signal */
	signal(SIGHUP, icbexit);
	signal(SIGTERM, icbexit);

	/* on an interrupt, verify the user wants to quit */
	signal(SIGINT, askquit);

	/* suspend on a stop signal */
	signal(SIGTSTP, suspend);
}


/* handle a stop signal */
/* this technique is something I learned from some other software */
/* someday I need to learn just why it works */

RETSIGTYPE
suspend()
{
	restoreterm();
	signal(SIGTSTP, SIG_DFL);
	sigsetmask(sigblock(0) &~ mask(SIGTSTP));
	kill(0, SIGTSTP);
	sigblock(mask(SIGTSTP));
	signal(SIGTSTP, suspend);
	icbterm();
	continued = 1;
}


/* verify that the user wants to quit */

RETSIGTYPE
askquit()
{
	int c;
	int oem;
	static char out[3] = { '\0', '\r', '\n' };

	signal(SIGINT, SIG_IGN);
	fflush(stdin);
	write(1, "\nReally Quit? ", 14);
	
	oem = echomode;
	noecho();
	if ((c = getchar()) == EOF)
		(void)icbexit();
	
	out[0] = c;
	write(1, out, 3);
	if (c == 'Y' || c == 'y')
		icbexit();

	fflush(stdout);
	fflush(stdin);
	signal(SIGINT, askquit);
	continued = 1;

	if (oem)
	{
		/* rl_redraw();*/
		//fuck rl_refresh_line();
		echo();
	}
}
