/* Copyright (c) 1990 by Carrick Sean Casey. */
/* For copying and distribution information, see the file COPYING. */

/* parse a line from the user */

#include <config.h>
#include <stdlib.h>
#include <string.h>
#include "icb.h"
#include "externs.h"

extern Tcl_Interp *interp;
char *nextword();

char *argv[10];
static char sbuf[1024];

parse(s)
char *s;
{
	char cmdbuf[1024], *p;
	int result;
	char *line = s;
	int len;
	char *dbuf = NULL;
	char *args;

	/* see if it's just message text */
	if (*s != gv.cmdchar || (*++s==gv.cmdchar)) {
		send_default(s,1);
		goto endparse;
	}

	/* check to see if it's a shell command */
	/* what a mess, but it works */
	if (*s == '!') {
		char *w;
		s++;

		if (gv.restricted) {
			sprintf(cgenbuff,"%s[=%sError%s=]%s c_shell: no shell commands to restricted mode%s",cequalbracket,cerror,cequalbracket,cgenmessage,csane);
		   putl(cgenbuff,
		    PL_SCR);
		 goto endparse;
		}

		if ((*s == ' ' || *s == '\t') && ((w = nextword(s)) != NULL)) {
			argv[0] = "c_shell"; argv[1] = w; argv[2] = NULL;
			c_shell(NULL, interp, 2, argv);
		} else if (*s != '\0') {
			argv[0] = "c_shell"; argv[1] = s; argv[2] = NULL;
			c_shell(NULL, interp, 2, argv);
		} else {
			argv[0] = "c_shell"; argv[1] = NULL;
			c_shell(NULL, interp, 1, argv);
		}
		goto endparse;
	}

	/* check to see if it's a tcl special */
	if (wordcmp(s, "tcl") == 0) {
		s += 4;
		strcpy(cmdbuf,s);
	} else {
		/* if not, quote the text */
		quoteify(s, cmdbuf);
	}

	putl(line, PL_LOG);

	/* call TCL to evaluate the string */
	result = Tcl_Eval(interp, cmdbuf);

	if (result == TCL_ERROR) {
		Tcl_Obj *objres = Tcl_GetObjResult(interp);
		char *resultstr = Tcl_GetStringResult(interp);
		args = findspace(s);
		sendcmd(s,args);
		goto endparse;
	}
		
	/* print the results */
	if (result == TCL_OK) {
		if (*interp->result != 0)
			putl(interp->result, PL_SCR | PL_LOG);
	} else {
		if (interp->result) {
			sprintf(mbuf, "%s[=%sError%s=]%s %s%s",cequalbracket,cerror,cequalbracket,cwarning,interp->result,csane);
			putl(mbuf, PL_SCR | PL_LOG);
		} else
			sprintf(cgenbuff,"%s[=%sError%s=]%s Unknown Tcl error%s",cequalbracket,cerror,cequalbracket,cwarning,csane);
			putl(cgenbuff, PL_SCR | PL_LOG);
	}

endparse:
	if (dbuf)
		free(dbuf);
	return;
}

#ifdef NEVER
/* compare the first word in s1 with s2 */
wordcmp(s1, s2)
char *s1;
char *s2;
{
	char *p = s1;
	int count = 0;
	while (*p != ' ' && *p != '\t' && *p != '\0') {
		count++;
		p++;
	}
	if (count)
		return(strncmp(s1, s2, count));
	else
		return(1);
}
#endif /* NEVER */

/* starting at a word, find the start of the next one */
char *nextword(s)
char *s;
{
	while (*s != ' ' && *s != '\t' && *s != '\0') s++;
	while ((*s == ' ' || *s == '\t') && *s != '\0') s++;
	if (*s == '\0')
		return(0);
	else
		return(s);
}
