/* Copyright 1990 by Carrick Sean Casey. */
/* For copying and distribution information, see the file COPYING. */

/* primitive to set icb internal variables */

#include <config.h>
#include <stdlib.h>
#include <string.h>
#include "icb.h"
#include "externs.h"

/* struct for variable table */
typedef struct Variable {
	char *name;
	int type;
	char *address;
} VARIABLE;


/* variable types */
#define V_CHAR		1
#define V_BOOLEAN	2
#define V_NONNEG	3
#define V_INT		4
#define V_STRING	5


/* list of usable variables */
VARIABLE vars[] = {
	{ "alert",	V_STRING,	(char *)&gv.alert },
	{ "autodel",	V_BOOLEAN,	(char *)&gv.autodel },
	{ "autoreg",	V_BOOLEAN,	(char *)&gv.autoregister },
	{ "beeps",	V_BOOLEAN,	(char *)&gv.beeps },
	{ "bufferlines",	V_NONNEG,	(char *)&gv.bufferlines },
	{ "commandchar",	V_CHAR,		(char *)&gv.cmdchar },
	{ "cute",		V_BOOLEAN,	(char *)&gv.cute },
	{ "editmode",	V_STRING,	(char *)&gv.editmode },
	{ "groupblanks",	V_BOOLEAN,	(char *)&gv.groupblanks },
	{ "history",	V_NONNEG,	(char *)&gv.phistory },
	{ "idle", V_STRING, (char *)&gv.idle },
	{ "logfile",	V_STRING,	(char *)&gv.logfile },
	{ "logreplay",	V_BOOLEAN,	(char *)&gv.logreplay },
	{ "pagesize",	V_INT,		(char *)&gv.pagesize },
	{ "pauseonshell",	V_BOOLEAN,	(char *)&gv.pauseonshell },
	{ "personalto", V_STRING,	(char *)&gv.personalto },
	{ "restricted",	V_BOOLEAN,	(char *)&gv.restricted },
	{ "reusepvalue", V_BOOLEAN,	(char *)&gv.reusepvalue },
	{ "tabreply",	V_BOOLEAN,	(char *)&gv.tabreply },
	{ "timedisplay",	V_STRING,	(char *)&gv.timedisplay },
	{ "verifyquit",	V_BOOLEAN,	(char *)&gv.verifyquit },
	{ NULL, 0, NULL }
};


oset(TARGLIST)
TARGDEFS
{
	static char *usage = "usage: oset var [value]";
	int x;

	if (argc == 1) {
		(void) listvars(NULL);
		return(TCL_OK);
	}

	else if (argc == 2) {
		if (listvars(argv[1]) < 0)
			TRETURNERR("oset: no such variable")
		else
			return(TCL_OK);
	}

	/* "restricted" is a special case */
	if (!strcmp(argv[1], "restricted")) {
		if (gv.restricted)
			TRETURNERR("oset: restricted mode can't be reset")
		else {
			restrict();
			return(TCL_OK);
		}
	}

	strcpy(mbuf,catargs(&argv[2]));

	for (x = 0; vars[x].name != NULL; x++) {
		if (strcmp(argv[1], vars[x].name) == 0) {
			if (setvar(interp, vars[x].type,
			vars[x].address, mbuf) < 0)
				return(TCL_ERROR);
			else
				break;
		}
	}

	if (vars[x].name == NULL) {
		sprintf(TRET, "oset: no such variable \"%s\"", argv[1]);
		return(TCL_ERROR);
	}

	return(TCL_OK);
}

ounset(TARGLIST)
TARGDEFS
{
	static char *usage = "usage: ounset var";
	int x;

	if (argc != 2)
	{
		TRETURNERR(usage);
	}

	/* "restricted" is a special case */
	if (!strcmp(argv[1], "restricted")) {
		if (gv.restricted)
		{
			TRETURNERR("ounset: restricted mode can't be reset")
		}
	}

	for (x = 0; vars[x].name != NULL; x++) {
		if (strcmp(argv[1], vars[x].name) == 0) {
			if (unsetvar(interp, vars[x].type, vars[x].address) < 0)
				return(TCL_ERROR);
			else
				break;
		}
	}

	if (vars[x].name == NULL) {
		sprintf(TRET, "ounset: no such variable \"%s\"", argv[1]);
		return(TCL_ERROR);
	}

	return(TCL_OK);
}


setvar(interp, type, address, s)
Tcl_Interp *interp;
int type;
char *address;
char *s;
{
	int zero = 0, one = 1;
	int tmp;
/*	char *p, *malloc(); */
	char *p;

	switch(type) {
	case V_CHAR:
		*address = *s;
		break;
	case V_INT:
		tmp = atoi(s);
		bcopy(&tmp, address, sizeof(int));
		break;
	case V_NONNEG:
		tmp = atoi(s);
		if (tmp < 0) {
			strcpy(TRET, "oset: only non-negative values allowed");
			return(-1);
		}
		bcopy(&tmp, address, sizeof(int));
		break;
	case V_BOOLEAN:
		if (!strncmp(s, "on", 2) ||
		   !strncmp(s, "true", 4) ||
		   *s == 'y' || *s == '1')
			{
			int *i_addr = (int *)address;
			*i_addr = 1;
			}
		else if (!strncmp(s, "off", 2) ||
			!strncmp(s, "false", 4) ||
			*s == 'n' || *s == '0')
			{
			int *i_addr = (int *)address;
			*i_addr = (int)0;
			}
		else {
			strcpy(TRET, "oset: only boolean values allowed");
			return(-1);
		}
		break;
	case V_STRING:
		tmp = strlen(s) + 1;
		if ((p = malloc(tmp)) == NULL) {
			strcpy(TRET, "oset: not enough memory to set string");
			return(-1);
		}
		bcopy(s, p, tmp);
		bcopy(&p, address, sizeof(p));
		break;
	}
	varsanity();
	return(0);
}

unsetvar(interp, type, address)
Tcl_Interp *interp;
int type;
char *address;
{
	switch(type) {
	case V_CHAR:
		*address = '\0';
		break;
	case V_STRING:
		{
		char **s_addr = (char **)address;
		*s_addr = "";
		break;
		}
	case V_INT:
	case V_NONNEG:
	case V_BOOLEAN:
		{
		int *i_addr = (int *)address;
		*i_addr = 0;
		}
		break;
	}
	varsanity();
	return(0);
}

listvars(name)
char *name;
{
	int x;

	if (name == NULL) {
		putl("VARIABLE      TYPE  VALUE", PL_SCR);
		for (x = 0; vars[x].name != NULL; x++)
			printvar(vars[x].name, vars[x].type, vars[x].address);
		return(0);
	}

	else
		for (x = 0; vars[x].name != NULL; x++)
			if (strcmp(name, vars[x].name) == 0) {
				putl("VARIABLE      TYPE  VALUE", PL_SCR);
				printvar(vars[x].name,
				  vars[x].type, vars[x].address);
				break;
			}

	if (vars[x].name == NULL)
		return(-1);
	else
		return(0);
}


printvar(name, type, address)
char *name;
int type;
char *address;
{
	int ival;

	switch(type) {
	case V_CHAR:
		sprintf(mbuf, "%-12s  char  '%c'", name, *address);
		break;
	case V_INT:
		bcopy(address, &ival, sizeof(int));
		sprintf(mbuf, "%-12s  int   %d", name, ival);
		break;
	case V_NONNEG:
		bcopy(address, &ival, sizeof(int));
		sprintf(mbuf, "%-12s  nneg  %d", name, ival);
		break;
	case V_BOOLEAN:
		sprintf(mbuf, "%-12s  bool  %s", name,
		  *(int *)address  ? "true" : "false");
		break;
	case V_STRING:
		sprintf(mbuf, "%-12s  str   \"%s\"", name, *(char **)address);
		break;
	default:
		sprintf(mbuf, "printvar unknown type %d for \"%8s\"",type,name);
		break;
	}
	putl(mbuf, PL_SCR);
}

/* perform a sanity check on some variables */
varsanity()
{
	if (gv.pagesize < 0) {
		getwinsize();
		gv.pagesize = ttyinfo.rows - 1;
		if (gv.pagesize < 0)
			gv.pagesize = 0;
	}
	if (gv.pagesize > 500)
		gv.pagesize = 500;

	if (gv.bufferlines > 5000)
		gv.bufferlines = 5000;

	if (strcmp(gv.editmode, "emacs")==0)
	{
		rl_emacs_editing_mode();
	}
	else 
	{
		rl_vi_editing_mode();
		gv.editmode = "vi";
	}

	if ( strcmp(gv.alert, "none")!=0 && strcmp(gv.alert, "all")!=0 &&
	     strcmp(gv.alert, "personal")!=0)
		gv.alert = "none";
		
}

restrict()
{
	gv.restricted = 1;
	tclrestrict();
}
