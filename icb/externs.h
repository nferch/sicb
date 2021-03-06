/* Copyright (c) 1989 by Carrick Sean Casey. All rights reserved. */
/* For copying and distribution information, see the file "copyright.h." */

/* external definitions for "icb" global variables */

/* defined in port.c */
extern char *hostname; 	/* default hostname for connects */
extern int portnumber;	/* default INET port number */

/* defined in split.c */
extern char *fields[MAX_FIELDS];	/* split fields */

/* global icb variables settable with oset */
extern GLOBS gv;
extern TTYINFO ttyinfo;
	
/* defined in globals.c */
extern char *what_version;		/* ICB client version */
#define icb_version (what_version+5)	/* . . . without leading what(1) key */
extern char *pp;		/* packet pointer */
extern char *pbuf;		/* packet buffer pointer */
extern char *mynick;		/* nickname of user */
extern char *myloginid;		/* login id */
extern char *mypass;		/* password of user */
extern char *mygroup;		/* requested group */
extern char *myserver;		/* server name */
extern char *myhost;		/* server host */
extern char *myshost;		/* server host */
extern char *mywogin;		/* server host */
extern int myport;		/* server port */
extern char *mbuf;		/* message buffer */
extern char *mbuf2;		/* message buffer */
extern int beeping;		/* 1 if beeping turned on */
extern connected;		/* whether we are validated or not */
extern char thishost[];		/* our hostname */
extern char continued;		/* did we bg then fg this job? */
extern int whoflg;		/* just do a who at startup */
extern time_t curtime;		/* current time */
extern int linenumber;
extern int echomode;		/* whether tty echo is on or off */
extern int match_exact;		/* used by histmatch() */

extern int m_whoheader;		/* who header output */
extern int m_groupheader;	/* group header output */
extern int m_nomesg;		/* turn off messages while in forum */
extern int m_watchtime;		/* using boring time format */

/* defined in ../readline/readline.c */
extern int last_command_was_kill;
