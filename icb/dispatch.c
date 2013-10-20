/* Copyright (c) 1990 by Carrick Sean Casey. */
/* For copying and distribution information, see the file COPYING. */

/* handle a packet from the server */

#include <config.h>
#include "icb.h"
#include "externs.h"

dispatch(pkt)
char *pkt;
{
	switch(*pkt) {

	case M_PING:
		sendpong();
		break;

	case M_BEEP:
		beep(++pkt);
		break;

	case M_PROTO:
		protomsg(++pkt);
		break;

	case M_LOGINOK:
		loginokmsg(++pkt);
		break;

	case M_OPEN:
		copenmsg(++pkt);
		break;

	case M_STATUS:
		statusmsg(++pkt);
		break;

	case M_ERROR:
		errormsg(++pkt);
		break;

	case M_IMPORTANT:
		importantmsg(++pkt);
		break;

	case M_EXIT:
		exitmsg();
		break;

	case M_CMDOUT:
		cmdoutmsg(++pkt);
		break;

	case M_PERSONAL:
		cpersonalmsg(++pkt);
		break;

	default:
		sprintf(mbuf, "%s[=%sError%s=]%s Invalid packet type %s\"%c\"%s",cequalbracket,cerror,cequalbracket,cerror,cpackettype,*pkt,csane);
		putl(mbuf, PL_ALL);
	}
}
