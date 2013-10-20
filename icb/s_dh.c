/* Copyright 1990 by Carrick Sean Casey. */
/* For copying and distribution information, see the file COPYING. */

/* primitive to do a diffie-hellman key agreement */

#include <config.h>
#include "icb.h"
#include "externs.h"
#include "icbcrypt.h"

s_dh(TARGLIST)
TARGDEFS
{
	char dhmessage[1024];
	static char *usage = "usage: s_dh nick";

	if (argc != 2 || !*argv[1])
		TRETURNERR(usage)

	sprintf(dhmessage,"{!DH:PV:%s}",dhGetPValue(argv[1],gv.reusepvalue));
	sendpersonal(argv[1],dhmessage,0);
	sprintf(dhmessage,"{!DH:PK:%s}",dhMakeKeyPair(argv[1]));
	sendpersonal(argv[1],dhmessage,0);
	sprintf(mbuf,"%cdh %s",gv.cmdchar,argv[1]);
	bufferadd(mbuf);
	return(TCL_OK);
}
