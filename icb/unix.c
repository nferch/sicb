/* Copyright (c) 1990 by Carrick Sean Casey. */
/* For copying and distribution information, see the file COPYING. */

/* This file contains routines that are unix dependent. */
/* Eventually, most unixisms should be moved here. */

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "icb.h"
#include "externs.h"
#include <pwd.h>

#ifdef HAVE_DIRENT_H
# include <dirent.h>
# define NAMLEN(dirent) strlen((dirent)->d_name)
#else
# define dirent direct
# define NAMLEN(dirent) (dirent)->d_namlen
# if HAVE_SYS_NDIR_H
#  include <sys/ndir.h>
# endif
# if HAVE_SYS_DIR_H
#  include <sys/dir.h>
# endif
# if HAVE_NDIR_H
#  include <ndir.h>
# endif
#endif


#undef stty
#undef gtty

#if ( defined(HAVE_SGTTY_H) || defined(HAVE_BSD_SGTTY_H) ) && ( ! defined(HAVE_TERMIO_H))
/* termio is preferred over sgtty, i guess */

#ifdef HAVE_BSD_SGTTY_H
#include <bsd/sgtty.h>
#else
#include <sgtty.h>
#endif

#define TTYSTRUCT sgttyb
#define BSD_TTYSTRUCT
#define stty(fd,buf) ioctl((fd),TIOCSETN,(buf))
#define gtty(fd,buf) ioctl((fd),TIOCGETP,(buf))

#else /* HAVE_SGTTY_H||BSD */

#ifdef HAVE_TERMIO_H

#include <termio.h>
#define TTYSTRUCT termio
#define SYSV_TTYSTRUCT
#define stty(fd,buf) ioctl((fd),TCSETA,(buf))
#define gtty(fd,buf) ioctl((fd),TCGETA,(buf))

#endif /* SYSV */

#endif /* HAVE_SGTTY_H */

char *getlogin();

struct TTYSTRUCT origtty;	/* holds the user's original term settings */

int badttyinfo = 0;	/* used when running under some weird modes */

/* get a user's login id */
/* returns pointer to ID on success, 0 on failure */

char *
getloginid()
{
	char *loginid;
	struct passwd *pw;

	if (((loginid = getlogin()) == NULL) || *loginid == '\0')
		if ((pw = getpwuid(getuid())) == NULL)
			loginid = NULL;
		else
			loginid = pw->pw_name;
	return(loginid);
}


/* stash the current time in curtime */
void
gettime()
{
	time(&curtime);
}


/* set line buffering for an open file pointer */
/* output will be flushed every newline */

linebuffer(fp)
FILE *fp;
{
#ifndef SYSV_TTYSTRUCT
	setlinebuf(fp);
#endif /* SYSV */
}


/* push a character into the terminal's input buffer */
pushback(c)
char c;
{
#ifndef SYSV_TTYSTRUCT
	if (ioctl(0, TIOCSTI, &c) < 0)
		perror("TIOCSTI ioctl failed");
#else /* SYSV */
	if (ungetc(c,stdin) == EOF)
		perror("ungetc() failed");
#endif /* SYSV */
}


/* get the original terminal settings */
/* stash them in origtty */

getterm()
{
#ifndef SYSV_TTYSTRUCT
	struct ltchars lt;
#endif /* SYSV */
	/* get tty settings */
	if (gtty(0,&origtty) < 0) {
		badttyinfo++;
		ttyinfo.erase = '\b';	/* ^H */
		ttyinfo.kill = '\025';	/* ^U */
	} else {
#ifndef SYSV_TTYSTRUCT
		ttyinfo.erase = origtty.sg_erase;
		ttyinfo.kill = origtty.sg_kill;
#else /* SYSV */
		ttyinfo.erase = origtty.c_cc[VERASE];
		ttyinfo.kill = origtty.c_cc[VKILL];
#endif /* SYSV */
	}

#ifndef SYSV_TTYSTRUCT
	/* get local special chars */
 	if (ioctl(0, TIOCGLTC, &lt) < 0) {
		ttyinfo.redraw = '\022'; /* ^R */
		ttyinfo.werase = '\027'; /* ^W */
	} else {
		ttyinfo.redraw = lt.t_rprntc;
		ttyinfo.werase = lt.t_werasc;
	}
#else /* SYSV */
	ttyinfo.redraw = '\022'; /* ^R */
	ttyinfo.werase = '\027'; /* ^W */
#endif /* SYSV */

	/* get the current window size */
	getwinsize();
}



/* set up terminal modes optimal for icb */

icbterm()
{
	struct TTYSTRUCT tty;

	if (badttyinfo)
		return;


	bcopy((char *)&origtty, (char *)&tty, (unsigned)sizeof(struct TTYSTRUCT));

	/* turn on cbreak - turn off echo */
#ifndef SYSV_TTYSTRUCT
	tty.sg_flags |= CBREAK;
	tty.sg_flags &= ~ECHO;
#else /* SYSV */
	tty.c_lflag  &= ~ICANON;
	tty.c_cc[VEOF] = 1;
	tty.c_lflag &= ~ECHO;
#endif /* SYSV */

	echomode = 0;
	/* set the new flags */
	if (stty(0, &tty) < 0) {
		perror("stty() failed");
		exit(-1);
	}
}


/* restore term to original settings */

restoreterm()
{
	if (badttyinfo)
		return;

	if (stty(0,&origtty) < 0) {
		perror("stty() failed");
		exit(-1);
	}
}

/* get the number of lines and columns for this tty */
/* stash them in origtty */

getwinsize()
{
#ifndef NO_WINDOWSIZE
	struct winsize win;

	/* get tty settings */
 	if (ioctl(0, TIOCGWINSZ, &win) < 0) {
		ttyinfo.rows = 24;
		ttyinfo.cols = 80;
	} else {
		ttyinfo.rows = win.ws_row;
		ttyinfo.cols = win.ws_col;
	}
#else
	/* for unixes that winsize or TIOCGWINSZ */
	ttyinfo.rows = 24;
	ttyinfo.cols = 80;
#endif
}

char *
tildexpand(s)
char *s;
{
	struct passwd *pw;
	char login[17];
	char *p = login;
	static char path[MAXNAMLEN+1];

	/* has to start with a tilde */
	if (*s++ != '~')
		return(s);

	/* skim off the login id */
	while (((*s >= 'A' && *s <= 'Z') || (*s >= 'a' && *s <= 'z') ||
	  index(".-_", *s)) && ((p - login) < 17))
		  *p++ = *s++;

	/* terminate string */
	*p = '\0';

	/* get home directory from password file */
	if ((pw = getpwnam(login)) == NULL)
		return(NULL);

	strcpy(path, pw->pw_dir);
	strcat(path, s);

	return(path);
}

clearargs(argc,argv)
int argc;
char *argv[];
{
	int x;
	char *cp;

	for (x = 1; x <= argc-1; x++) {
		cp = argv[x];
		while(*cp)
			*cp++ = ' ';
	}
}

void
echo()
{

	struct TTYSTRUCT tty;

	/* get current flags */
	if (gtty(0,&tty) <0)
	{
		perror("gtty failed.");
		exit(-1);
	}

	/* turn on echo */
#ifndef SYSV_TTYSTRUCT
	tty.sg_flags |= ECHO;
#else /* SYSV */
	tty.c_lflag  |= ECHO;
#endif /* SYSV */

	echomode = 1;

	/* set the new flags */
	if (stty(0, &tty) < 0) {
		perror("stty failed");
		exit(-1);
	}
}

void
noecho()
{

	struct TTYSTRUCT tty;

	/* get current flags */
	if (gtty(0,&tty) <0)
	{
		perror("gtty failed.");
		exit(-1);
	}

	/* turn off echo */
#ifndef SYSV_TTYSTRUCT
	tty.sg_flags &= ~ECHO;
#else /* SYSV */
	tty.c_lflag  &= ~ECHO;
#endif /* SYSV */

	echomode = 0;

	/* set the new flags */
	if (stty(0, &tty) < 0) {
		perror("stty failed");
		exit(-1);
	}
}
