/* Copyright (c) 1989 by Carrick Sean Casey. All rights reserved. */
/* For copying and distribution information, see the file COPYING. */

/*

Given a string, return an internet address.

This accepts address in the form of 

num.num.num.num
[num.etc]
machine.dom.dom.dom

*/

#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
/*#include "select.h"*/
/*#include "murgil.h"*/
/*#include "externs.h"*/

struct in_addr *
_getaddress(s)
char *s;
{
	static struct in_addr iaddr;
	struct hostent *hp;

	/* handle case of "[num.num.num.num]" */
	if (*s == '[') {
		char *p;

		s++;
		for (p = s; *p != '\0'; p++)
			if (*p == ']') {
				*p = '\0';
				break;
			}
	}

	/* handle case of "num.num.num.num" */
	if (*s >= '0' && *s <= '9') {
		if ((iaddr.s_addr = (unsigned long)inet_addr(s)) == -1)
			return(0);
		return(&iaddr);
	} 

	/* handle a symbolic address */
	if ((hp = gethostbyname(s)) == (struct hostent *) 0)
		return(0);

	/* copy address into inet address struct */
	bcopy(hp->h_addr, (char *) &iaddr.s_addr, (unsigned int) hp->h_length);

	return((struct in_addr *) &iaddr);
}
