/* Copyright (c) 1988 Carrick Sean Casey. All rights reserved. */

/* start listening on our official port */
/* returns socket for new connects */

#include <stdio.h>
#include <string.h>
#include "config.h"
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "select.h"
#include "ipcf.h"
#include "externs.h"
 
makenewport(host_name, port_number)
char *host_name;
int port_number;
{
	struct sockaddr_in saddr;
	struct hostent *hp;
	int one = 1;
	int s;

	/* get the server host inet address */
	if ((hp = gethostbyname(host_name)) < (struct hostent *) 0) {
		fail = "makenewport: gethostbyname";
		return(-1);
	}

	/* insert host_name into address */
	bzero((char *) &saddr, sizeof(saddr));
	bcopy(hp->h_addr, (char *) &saddr.sin_addr, hp->h_length);

	/* fill in socket domain and port number */
	saddr.sin_family = hp->h_addrtype;
	saddr.sin_port = htons(port_number);

	/* create a socket */
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		fail = "makenewport: gethostbyname";
		return(-1);
	}

	/* bind it to the inet address */
	if (bind(s, (struct sockaddr *) &saddr, sizeof(saddr)) < 0) {
		fail = "makenewport: bind";
		perror("perror says:");
		return(-1);
	}

	/* start listening for connections */
	listen(s, 255);

	/* make it non-blocking */
#if !defined(SYSV) && !defined(linux)
	if (fcntl(s, F_SETFL, FNDELAY) < 0) {
#else /* SYSV */
	if (fcntl(s, F_SETFL, O_NDELAY) < 0) {
#endif /* SYSV */
		fail = "makenewport: fcntl";
		return(-1);
	}

	/* force occasional connection check */
	if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR,
	  (char *)&one, sizeof(one)) < 0) {
		puts("makeport:setsockopt (makeport)");
/*		return(-1);*/
	}

	/* allow us to handle problems gracefully */
	signal(SIGPIPE, SIG_IGN);

	port_fd = s;
	return(0);
}
