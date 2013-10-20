/* Copyright 1990 by Carrick Sean Casey. */
/* For copying and distribution information, see the file COPYING. */

/* primitive to send a personal message */

#include <config.h>
#include <string.h>
#include "icb.h"
#include "externs.h"

s_beep(TARGLIST)
TARGDEFS
{
	static char *usage = "usage: s_beep nick";

	/* check the arg count */
	if (argc != 2 || !*argv[1])
		TRETURNERR(usage)

	sendcmd("beep", argv[1]);
	histput(argv[1]);
	sprintf(mbuf,"%cbeep %s",gv.cmdchar,argv[1]);
	bufferadd(mbuf);
	return(TCL_OK);
}
