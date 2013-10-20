
/* return a remote connection's host name, or NULL if it */
/* can't be looked up */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

char *
getremotename(s)
int s;			/* connected socket fd */
{
	static char rname[24];
	struct hostent *host;
#if defined(linux)
#define PEER_CAST (struct sockaddr *)
#define HOST_CAST (char *)
#else
#define PEER_CAST
#define HOST_CAST
#endif
	struct sockaddr_in rs;
	int rs_size = sizeof(rs);
/* temporary hack */
char *debug_c;
long debug_l, inet_addr();
	char *inet_ntoa();

	/* get address of remote user */
	if (getpeername(s, PEER_CAST &rs, &rs_size) < 0) {
		perror("server: getpeername failed");
		return(NULL);
	}
/* temporary hack */
	debug_c = inet_ntoa(rs.sin_addr);
	debug_l = inet_addr(debug_c);

	/* get hostname from table */
	if ((host = gethostbyaddr(HOST_CAST &rs.sin_addr.s_addr,
	  sizeof(rs.sin_addr.s_addr), AF_INET)) == 0) {
#ifdef NEVER
/* temporary hack */
	if ((host = gethostbyaddr(&debug_l, sizeof(debug_l), AF_INET)) == 0) {
		/* not there, return address in numerical format */
		sprintf(rname, "[%s]", inet_ntoa(debug_l));
#endif
		return(inet_ntoa(rs.sin_addr.s_addr));
	} else
		return(char *)(host->h_name);
}
