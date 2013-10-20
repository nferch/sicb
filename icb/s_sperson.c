/* Copyright 1990 by Carrick Sean Casey. */
/* Copyright 1990-1993,1994 by Mark J. Reed */
/* For copying and distribution information, see the file COPYING. */

/* primitive to send a personal message */

#include <config.h>
#include <stdlib.h>
#include <string.h>
#include "icb.h"
#include "externs.h"
#include "getswitch.h"

s_spersonal(TARGLIST)
TARGDEFS
{
	static char *usage = "usage: s_personal [-permanent|-clear] nick message ...";
	static char *switches[] = { "permanent", "clear", NULL };
	static char *personalto = NULL;
	char nick[MAX_NICKLEN+1];
	int permanent = 0;
	int clearflag = 0;
	char *sw;

	/* check for switches */
	switchind = 1;
	while (sw = getswitch(argc, argv, switches))
	{
		switch (sw[0])
		{
			case 'p':
				if (clearflag)
				{
					TRETURNERR("Only one of -permanent or -clear may be specified.");
				}
				permanent = 1;
				break;

			case 'c':
				if (permanent)
				{
					TRETURNERR("Only one of -permanent or -clear may be specified.");
				}
				clearflag = 1;
				break;
		}
	}

	/* check the arg count */
	if ((argc-switchind+1 < 2) && (!clearflag))
	{
		TRETURNERR(usage)
	}

	if (permanent)
	{
		if (personalto)
			free(personalto);
		personalto = (char *)malloc(strlen(argv[switchind])+1);
		strcpy(personalto, argv[switchind]);
		gv.personalto = personalto;
		if (argc-switchind+1==2)
			return TCL_OK;
	}

	if (clearflag)
	{
		gv.personalto = "";
		if (argc-switchind+1<=2)
			return TCL_OK;
	}

	if (!argv[switchind])
		return TCL_ERROR;

	strncpy(nick,argv[switchind],MAX_NICKLEN);
	nick[MAX_NICKLEN]='\0';
	ssendpersonal(nick,catargs(&argv[switchind+1]),0);
	return(TCL_OK);
}


ssendpersonal(nick, text, echoflag)
char *nick;
char *text;
int echoflag;
{
	char *buf;
	int pl_flags;

	switch (echoflag)
	{
		case 0:
			pl_flags = PL_BUF;
			break;

		case -1: 
			pl_flags = 0;
			break;

		default:
			pl_flags = PL_BUF | PL_SCR;
	}

	if (!nick)
		return;

	if (!text || !*text)
	{
		do
		{
			text = icb_getline(">> ",0);

			if (!text || !*text )
			{
				sprintf(cgenbuff,"%s[=%sEmpty message not sent%s=]%s",cequalbracket,cwarning,cequalbracket,csane);
				putl(cgenbuff, PL_SCR);
				return;
			}
			printf("\b\b\b");
		}
		while (!*text);
	}

	histput(nick);

	buf = (char *)malloc(strlen(nick)+strlen(text)+5);
	if (!buf)
	{
		sprintf(cgenbuff,"%s[=%sError%s=]%s No memory to build packet%s",cequalbracket,cerror,cequalbracket,cgenmessage,csane);
		putl(cgenbuff,PL_SCR);
		return 0;
	}

	sprintf(buf,"%cm %s %s", gv.cmdchar, nick, text);
	putl(buf, pl_flags);
	sendcmd("menc", buf+3);
}
