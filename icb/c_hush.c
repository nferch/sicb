/* Copyright 1990 by Carrick Sean Casey. */
/* For copying and distribution information, see the file COPYING. */

/* primitive to hush abusive users */

#include <config.h>
#include "icb.h"
#include "externs.h"

STRLIST *hushhead = NULL, *hushtail = NULL;

c_hush(TARGLIST)
TARGDEFS
{
	static char *usage = "usage: c_hush [nick]";

	if (argc == 1 || !*argv[1])
		hushlist();
	else
		if (!ishushed(argv[1])) {
			if (hushadd(interp, argv[1]) < 0)
				return(TCL_ERROR);
		} else 
			if (hushdelete(interp, argv[1]) < 0)
				return(TCL_ERROR);
	return(TCL_OK);
}


hushadd(interp, nick)
Tcl_Interp *interp;
char *nick;
{
	STRLIST *hp;

	/* skip whitespace */
	while (*nick == ' ' || *nick == '\t') nick++;

	if (*nick == '\0') {
		strcpy(TRET, "c_hush: can't hush a null nickname");
		return(-1);
	}

	if (strlen(nick) > MAX_NICKLEN)
		*(nick + MAX_NICKLEN) = '\0';

	if ((hp = strmakenode(strlen(nick))) == NULL) {
		strcpy(TRET, "c_hush: out of memory");
		return(-1);
	}
	strcpy(hp->str, nick);
	strlinkalpha(hp, &hushhead, &hushtail, 1);
	sprintf(mbuf, "%s[=%sHush%s=]%s %s %sadded to hush list.%s", cequalbracket,cstatus,cequalbracket,cusername,hp->str,cgenmessage,csane);
	putl(mbuf, PL_SL);
	return(0);
}

hushdelete(interp, nick)
Tcl_Interp *interp;
char *nick;
{
	STRLIST *s;

	if ((s = strgetnode(nick, hushhead, 1)) == NULL) {
		sprintf(TRET, "c_hush: %s is not being hushed", nick);
		return(-1);
	}
	sprintf(mbuf, "%s[=%sHush%s=]%s %s%s deleted from hush list.%s",cequalbracket,cstatus,cequalbracket,cusername,nick,cgenmessage,csane);
	putl(mbuf, PL_SL);
	strunlink(s, &hushhead, &hushtail);
	free(s);
	return(0);
}

hushlist()
{
	STRLIST *p;
	char *s, *t;

	if (hushhead)
		putl("Hushed nicknames:", PL_SCR);
	else {
		sprintf(cgenbuff,"%s[=%sHush%s=]%s You are not currently hushing anyone.%s",cequalbracket,cwarning,cequalbracket,cgenmessage,csane);
		putl(cgenbuff, PL_SCR);
		return;
	}

	s = mbuf;
	for (p = hushhead; p; p = p->next) {
		if (s != mbuf)
			*s++ = ' ';
		for (t = p->str; *t != '\0'; t++)
			*s++ = *t;
		if (s-mbuf > 60) {
			*s = '\0';
			putl(mbuf, PL_SCR);
			s = mbuf;
		}
	}
	if (s != mbuf) {
		*s = '\0';
		putl(mbuf, PL_SCR);
	}
}

ishushed(nick)
char *nick;
{
	STRLIST *p;

	for (p = hushhead; p; p = p->next)
		if (!strcasecmp(p->str, nick))
			return(1);
	return(0);
}
