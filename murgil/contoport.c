/* Copyright (c) 1988 by Carrick Sean Casey. All rights reserved. */

/* connect to a port */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "select.h"
#include "ipcf.h"
#include "externs.h"

struct in_addr *_getaddress();

connecttoport(char *host_name,int port_number, char *sourcehost)
{
	int s;
	struct sockaddr_in saddr;
	struct in_addr *addr;
	struct sockaddr_in temp;
	struct hostent *hp;
	unsigned long addy;

	fprintf(stderr,"Trying to connect to port %d of host %s.\n", port_number,
				host_name);

	/* get the client host inet address */
	if ((addr = _getaddress(host_name)) == 0) {
		fail = "can't lookup server INET address";
		return(-1);
	}

	/* insert hostname into address */
	bzero((char *) &saddr, sizeof(saddr));
	bcopy((char *) addr, (char *) &saddr.sin_addr, 4);

	/* fill in socket domain and port number */
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons((u_short)port_number);
	bzero (&temp, sizeof(struct sockaddr_in));   
	temp=saddr;

	/* nf changes start here */
	/*	if (temp.sin_addr.s_addr=inet_addr("206.190.29.177") < 0) {
			printf("inet_addr failed!\n");	
		}
		printf("address is %s\n",inet_ntoa(temp.sin_addr.s_addr));*/

	if (sourcehost) {
		if ((hp = gethostbyname(sourcehost)) == NULL) {
			fail = "makenewport: gethostbyname";
			printf("can't resolve %s!\n",sourcehost);
			return(-1);
		} 
		bcopy(hp->h_addr, (char *) &(temp.sin_addr), hp->h_length);
		temp.sin_family=hp->h_addrtype;
		temp.sin_port=0;
		printf("Attempting to bind from %s...\n",sourcehost); 
	}
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		fail = "couldn't create socket";
		fprintf(stderr,"icb: couldn't connect to %s server.\n",host_name);
		return(-1);
	}
	if (sourcehost) {
		if (bind (s,(struct sockaddr *) &temp,sizeof(temp)) <0) {
			printf("bind failed! \n");
		}
	}
	/* create a socket */

	/* connect it to the server inet address */
	if (connect(s, (struct sockaddr *) &saddr, sizeof(saddr)) < 0) {
		fail = "couldn't connect to server (not running?)";
		fprintf(stderr,"icb: couldn't connect to %s server.\n",host_name);
		return(-1);
	}

	/* make it non-blocking */
#ifndef FNDELAY
#define FNDELAY 0
#endif
#ifndef O_NDELAY
#define O_NDELAY 0
#endif
#ifndef O_NONBLOCK
#define O_NONBLOCK 0
#endif
#ifdef hpux
#define FNDELAY 0
#define O_NDELAY 0
#endif
	if (fcntl(s, F_SETFL, FNDELAY | O_NDELAY | O_NONBLOCK) < 0) {
		fail = "contoport: fcntl";
		return(-1);
	}

	port_fd = s;
	return(0);
}
