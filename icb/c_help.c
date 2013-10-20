/* Copyright 1990 by Carrick Sean Casey. */
/* For copying and distribution information, see the file COPYING. */

/* primitive to display some help */

#include <config.h>
#include <string.h>
#include "icb.h"
#include "externs.h"

extern *helpkeys[], *helptopics[], **helptexts[];

c_help(TARGLIST)
TARGDEFS
{
	static char *usage = "usage: c_help [subject]";

	if (argc > 2)
		TRETURNERR(usage)

	if (argc == 1 || !*argv[1] ) {
		helptoc();
		return(TCL_OK);
	}

	else if (argc == 2) {
		if (helpme(argv[1]) < 0)
			TRETURNERR("c_help: no help for that subject")
		else
			return(TCL_OK);
	}

	else
		TRETURNERR(usage)
}

/* list the table of contents for help */

helptoc()
{
	int x;

	putl("[=Help table of contents=]", PL_SL);
	putl("", PL_SL);
	putl("Help is available for the following subjects:", PL_SL);
	putl("", PL_SL);

	for (x = 0; helpkeys[x] != 0; x++) {
		sprintf(mbuf, "%-16s %s", helpkeys[x], helptopics[x]);
		putl(mbuf, PL_SL);
	}

	putl("", PL_SL);
	sprintf(mbuf,
	 "To get help on a particular subject: %chelp subject", gv.cmdchar);
	putl(mbuf, PL_SL);
	sprintf(mbuf,
	 "To get a list of commands: %ccommands", gv.cmdchar);
	putl(mbuf, PL_SL);
	putl("", PL_SL);
	putl("[=End of help=]", PL_SL);
}


/* get help on a particular subject */
/* returns 0 upon success */
/* returns -1 if no help for given subject */

helpme(subject)
char *subject;
{
	int x;
	char **text;

	for (x = 0; helpkeys[x] != 0; x++)
		if (strcasecmp(subject, (char *) helpkeys[x]) == 0) {
			text = (char **)helptexts[x];
			sprintf(mbuf, "[=Help for %s=]", subject);
			putl(mbuf, PL_SL);
			putl("", PL_SL);
			for(x = 0; text[x] != NULL; x++)
				putl(text[x], PL_SL);
			putl("", PL_SL);
			putl("[=End of help=]", PL_SL);
			return(0);
		}
	return(-1);
}
