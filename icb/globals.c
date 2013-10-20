/* Copyright (c) 1990 by Carrick Sean Casey. */
/* Copyright (c) 1990-1993, 1994 Mark J. Reed */
/* For copying and distribution information, see the file COPYING. */

/* icb global variables */

#include <config.h>
#include "icb.h"
#include "../murgil/ipcf.h"
#include "../version.h"

/* non-global definitions */
char packetbuffer[USER_BUF_SIZE];		/* packet buffer */
char messagebuffer[1024];		/* generic large buffer */
char messagebuffer2[256];		/* generic large buffer */
char *wholist[50];

/* global defs */
char *what_version = GICBVERSION;	/* ICB Client Version */
char *pp = packetbuffer;	/* packet pointer */
char *pbuf = packetbuffer + 1;	/* packet buffer pointer */
char *mbuf = messagebuffer;	/* message buffer */
char *mbuf2 = messagebuffer2;	/* message buffer */
char *mynick= NULL;		/* nickname of user */
char *myloginid = 0;		/* login id of user */
char *mypass = "";		/* password of user */
char *mygroup = "";		/* requested group */
char *myserver = NULL;		/* name of server */
char *myhost   = NULL;		/* hostname of server */
char *myshost   = NULL;		/* hostname of server */
char *mywogin   = NULL;		/* hostname of server */
int  myport = 0;		/* string form of port number */
int beeping = 0;		/* 1 if beeping turned on */
int connected = 0;		/* 1 when server validates us */
char thishost[132];		/* our hostname */
char continued = 0;		/* did we bg then fg this job? */
int whoflg = 0;			/* just do a who at startup */
time_t curtime;			/* current time */
int linenumber = 0;		/* current screen line number */
int match_exact = 0;		/* used by histmatch() */


/* flags set in .forumrc */
int m_whoheader = 1;	/* who header output */
int m_groupheader = 1;	/* group header output */
int m_nomesg = 0;	/* turn off messages while in forum */
int m_watchtime = 0;	/* using boring time format */

TTYINFO ttyinfo;

GLOBS gv = {
	CMDCHAR,	/* cmdchar */
	0,		/* verifyquit */
	0,		/* pauseonshell */
	1,		/* interactive */
	0, 		/* paging */
	15, 		/* phistory */
	250,		/* bufferlines */
	0,		/* groupblanks */
	"icb.log",	/* logfile */
	0,		/* restricted */
	"normal",	/* timedisplay */
	1,		/* beeps */
	"vi",		/* editmode */
	0,		/* tabreply */
	1, 		/* cute */
	0,		/* autodel */
	"",		/* personalto */
	0,		/* logreplay */
	"none",		/* alert */
	0,		/* autoregister */
	GICB_DEFAULT_NOIDLE
};

int echomode = 1;	/* is echo on or off? */
