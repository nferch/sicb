/* Copyright (c) 1990 by Carrick Sean Casey. */
/* For copying and distribution information, see the file COPYING. */

#include <config.h>
#include "../protocol.h"

#ifdef HAVE_TCL_H
#include <tcl.h>
#endif

#ifdef HAVE_TCL8_0_TCL_H
#include <tcl8.0/tcl.h>
#endif

#ifdef HAVE_TCL8_1_TCL_H
#include <tcl8.1/tcl.h>
#endif

#ifdef HAVE_TCL8_2_TCL_H
#include <tcl8.2/tcl.h>
#endif

#include <stdio.h>
#include <sys/param.h>
#include "color.h"
#ifdef NEED_CTYPE
#include <ctype.h>
#endif

// char *malloc();

/* command usage information */

typedef struct Usage {
	char name[9];
	char type;
	char args[128];
	char usage[128];
	struct Usage *prev, *next;
} USAGE;

/* special global variables */

typedef struct {
	char cmdchar;		/* character to prefix a command */
	int verifyquit;		/* verify a quit always */
	int pauseonshell;	/* make user hit ret on shell return */
	int interactive;	/* nonzero if interactive session */
	int pagesize;		/* lines per page */
	int phistory;		/*  */
	int bufferlines;	/* lines in the review buffer */
	int groupblanks;	/* print a blank before each group */
	char *logfile;		/* name of session log file */
	int restricted;		/* nonzero if in restricted mode */
	char *timedisplay;	/* how time is displayed */
	int beeps;		/* whether beeps are audible */
	char *editmode;		/* input editing mode */
	int tabreply;		/* should replies go into the tab history? */
	int cute;		/* cute messages when a blank line is entered */
	int autodel;		/* automatically delete nicks from the tab
			        history that produce a "not logged on" error */
	char *personalto;	/* 'permanent' personal message recipient */
	int logreplay;		/* should replays go into the log by default? */
	char *alert;		/* alert mode */
	int autoregister;	/* prompt for password when [=Register=] msg
				   received? */
	char *idle;
	int reusepvalue;	/* toggles use of saved DH p value */
} GLOBS;

/* info on the user's tty */

typedef struct {
	char erase;		/* tty erase char */
	char kill;		/* tty kill char */
	char redraw;		/* tty reprint line char */
	char werase;		/* tty word erase char */
	int  rows;		/* tty word erase char */
	int  cols;		/* tty word erase char */
} TTYINFO;

/* a linked list of strings */

typedef struct Strlist {
	struct Strlist *next, *prev;
	char str[1];
} STRLIST;

/* misc */

#define CMDCHAR		'/'	/* default char for prefixing commands */
#define PROMPTKAR	'>'
#define PERMCMDCHAR	'\\'	/* like above, but can't be changed */
#define GICB_DEFAULT_NOIDLE "She blinded me with science."

/* putline flags */
#define PL_SCR		01	/* text may go to screen */
#define PL_LOG		02	/* text may go to session log */
#define PL_BUF		04	/* text may go to review buffer */
#define PL_GLOG         010 
#define PL_SL		(PL_SCR|PL_LOG)
#define PL_ALL		(PL_SCR|PL_LOG|PL_BUF)

/* useful defs for modules using TCL code */

#define TARGLIST clientData, interp, argc, argv
#define TARGDEFS char *clientData; Tcl_Interp *interp; int argc; char *argv[];
#define TRET	interp->result
#define TRETURNERR(x) { if (TRET) { strcpy(TRET, (x)); } return(TCL_ERROR); }

/* include function prototypes if ANSI compiler */
/* otherwise, include file that shows return type for each function */

#if defined __STDC__ || defined __GNUC__
#include "protos.h"
#else
#include "funcs.h"
#endif

#define MAX_HOSTLEN 256
#define MAX_PASSLEN 256

#define PERSONALSL ".icbservrc"

struct server
{
	char name[MAX_HOSTLEN], host[MAX_HOSTLEN];
	int port;
};
