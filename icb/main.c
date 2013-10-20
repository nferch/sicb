/* Portions copyright (c) 1990 by Carrick Sean Casey. */
/* Portions copyright (c) 1990-1993, 1994 Mark J. Reed */
/* For copying and distribution information, see the file COPYING. */


#include <config.h>
#include "icb.h"
#include "externs.h"
#include <sys/wait.h>
#ifdef linux
#include <sys/dir.h>
#else
#endif
#include "getswitch.h"

char *optv[] = { "clear", "list", "who", "restricted", "nickname:", "group:",
		 "host:", "port:", "password:", "server:", "N:", "color", "ahost:", 
		 "wogin:", "help/",
		(char *)NULL };

void usage(name, ret)
char *name;
int ret;
{
fprintf(stderr,"Usage: %s [switches]\n",name);
fprintf(stderr," Switches may be abbreviated.  They are:\n");
fprintf(stderr,"  -nickname nick\tsign on with nickname nick.\n");
fprintf(stderr,"  -password pass\tset password to pass.\n");
fprintf(stderr,"  -password -\t\tprompt for password.\n");
fprintf(stderr,"  -group group\t\tsign on into group group.\n");
fprintf(stderr,"  -server name\t\tconnect to server named name.\n");
fprintf(stderr,"  -ahost host\t\tconnect to server using source host host\n");
fprintf(stderr,"  -host host\t\tconnect to server on host host.\n");
fprintf(stderr,"  -port port\t\ttry to connect to port port.\n");
fprintf(stderr,"  -list\t\t\tlist known servers, in order.\n");
fprintf(stderr,"  -clear\t\twipe args from command line.\n");
fprintf(stderr,"  -who\t\t\tsee who's on; don't sign on.\n");
fprintf(stderr,"  -restricted\t\trestricted mode\n");
fprintf(stderr,"  -wogin\t\ticb login user\n");
fprintf(stderr,"  -color\t\tenable ANSI color mode (disabled by default)\n" );
fprintf(stderr,"  -help\t\t\tdisplay this message.\n");
fprintf(stderr," Note: specifying a password on the command line implies -clear.\n");
exit(ret);
}


main (argc, argv)
int argc;
char **argv;
{
	char *s;
	char *getenv(), *getpass(), *getloginid();
	int restrictflg = 0;
	int clearargsflg = 0;
	int listflg = 0;
	int colorflag = 0;
	struct server *serverdata;
	int try;
	int ret = 1;
	static char nick[MAX_NICKLEN+1];
	static char group[MAX_NICKLEN+2]; /* to allow room for @user */
	static char server[MAX_NICKLEN+1];
	static char host[MAX_HOSTLEN+1];
	static char pass[MAX_PASSLEN+1];
	static char shost[MAX_HOSTLEN+1];
	static char lhost[MAX_HOSTLEN+1];
	static char wogin[MAX_NICKLEN+1];

	mynick = NULL;
	mygroup = "1";
	myserver = NULL;
	myhost = NULL;
	myport = ICBPORT;
	myshost = NULL;
	mywogin=NULL;

	switcherr=0;

	nocolors();

	while ((s = getswitch(argc, argv, optv)) != NULL)
	{

		switch (s[0]) {

		case 'c':
			switch (s[1])
			{
			case 'l':
				clearargsflg++;
				break;
			case 'o':
				colorflag = 1;
				break;
			}
			break;

		case 'g':
			strncpy(group, switcharg, MAX_NICKLEN+1);
			group[MAX_NICKLEN+1] = '\0';
			mygroup=(char*) malloc(strlen(group));	
			strcpy(mygroup,group);
			break;
		case 'a':
			strncpy(shost,switcharg,MAX_HOSTLEN-1),
			shost[MAX_HOSTLEN-1]='\0';
			myshost=shost;
			break;
		case 's':
		case 'N':
			strncpy(server, switcharg, MAX_NICKLEN);
			server[MAX_NICKLEN] = '\0';
			myserver = server;
			break;

		case 'n':
			strncpy(nick, switcharg, MAX_NICKLEN);
			nick[MAX_NICKLEN] = '\0';
			mynick = nick;
			break;

		case 'l':
			listflg++;
			gv.interactive = 0;
			break;

		case 'p':
			switch (s[1])
			{
			case 'o':
				myport = atoi(switcharg);
				break;
			case 'a':
				strncpy(pass, switcharg, MAX_PASSLEN);
				pass[MAX_PASSLEN]='\0';
				mypass = pass;
				if (pass[0] == '-')
				{
					mypass = getpass("Password:");
				}
				clearargsflg++;
				break;
			}
			break;
				
		case 'w':
			switch(s[1]) {
				case 'h':	
				whoflg++;
				gv.interactive = 0;
				break;

				case 'o':
				strncpy(wogin, switcharg, MAX_NICKLEN+1);
				wogin[MAX_NICKLEN+1] = '\0';
				mywogin = wogin;
				break;
			}

		case 'r':
			restrictflg = 1;
			break;

		case 'h':
			if (s[1]=='o')
			{
				strncpy(host,switcharg,MAX_HOSTLEN-1);
				host[MAX_HOSTLEN-1]='\0';
				myhost = host;
				break;
			}
			usage(argv[0],0);
			break;
			
		case '?':
			switch (s[1])
			{
			case 'a':
				fprintf(stderr,"%s: %s ambiguous.\n", argv[0],
					switcharg);
				usage(argv[0],1);

			case 'n':
				fprintf(stderr,"%s: %s requires an argument.\n",
					argv[0], switcharg);
				usage(argv[0],1);

			default:;
			}

		
			switch (*switcharg++)
			{
			case 'g':
				strncpy(group, switcharg, MAX_NICKLEN+1);
				group[MAX_NICKLEN+1] = '\0';
				mygroup = group;
				break;

			case 's':
			case 'N':
				strncpy(server, switcharg, MAX_NICKLEN);
				server[MAX_NICKLEN] = '\0';
				myserver = server;
				break;

			case 'n':
				strncpy(nick, switcharg, MAX_NICKLEN);
				nick[MAX_NICKLEN] = '\0';
				mynick = nick;
				break;

			case 'h':
				strncpy(host,switcharg,MAX_HOSTLEN-1);
				host[MAX_HOSTLEN-1]='\0';
				myhost = host;
				break;

			default:
				usage(argv[0],1);
				break;
			}
			break;


		default:
			usage(argv[0],1);
			break;
		}
	}

	if (listflg)
	{
		listservers();
		exit(0);
	}

	if (clearargsflg)
		clearargs(argc, argv);

	if (colorflag)
		colorinit();

	/* PLEASE DO NOT FAKE LOGIN IDS - IT IS AGAINST ICB RULES */

/*	if ((myloginid = getloginid()) == NULL)
	{
		puts("Error - can't determine your login ID.");
		exit(-1);
	}*/
	if(mywogin) 
		myloginid=mywogin;
	else if(getenv("ICBUSER")) 
		myloginid = getenv("ICBUSER");
	else {
		if ((myloginid = getloginid())==NULL) {
			puts ("Error - can't determine your login ID.");
			exit(-1);
		}
}
	/* get our nickname */
	if (mynick == NULL && (mynick = getenv("ICBNAME")) == NULL &&
	  (mynick = getenv("FNNAME")) == NULL &&
	  (mynick = getenv("FORUMNAME")) == NULL)
		mynick = myloginid;
	
	/* can you believe this program uses random numbers? */
	srand((unsigned)time(0));


	/* initialize everybody and say hello. */
	write(2, icb_version, strlen(icb_version));
	write(2, "\n", 1);

	if (gv.interactive) {
		tclinit();
		readlineinit();
		if (restrictflg && !gv.restricted)
			restrict();
	}

	/* try to connect to port - if it fails, start server if we are on */
	/* same machne then try again */

	if (myserver)
	{
		serverdata = getservbyname(myserver);
		if (!serverdata)
		{
			fprintf(stderr,"%s: %s: server not found.\n",
				argv[0], myserver);
			fprintf(stderr,"(use -h to specify a hostname instead of a server name)\n");
			exit(1);
		}
		try = 0;
	}
	else
	{
		serverdata = getserver();
		try = 1;
	}

	if (!myhost)
	{
		if (!serverdata)
			exit(-1);

		myhost = serverdata->host;
		myport = serverdata->port;
	}
		
/* fprintf(stderr,"Connecting to %s:%d from %s.\n",myhost,myport,myshost); */
	if (connecttoport(myhost, myport,myshost) < 0)
	{
			connected = 0;
			if (try)
			{
				while ((serverdata = getserver())!=NULL)
				{
fprintf(stderr,"Connecting to %s:%d\n",serverdata->host,serverdata->port);
					if (connecttoport(serverdata->host, serverdata->port) == 0)
					{
						connected = 1;
						break;
					}
				}
			}

			if (!connected)
				exit(1);
	}
 

	if (gv.interactive) {
		/* stash old term settings */
		getterm();

		/* set up terminal mode */
		icbterm();

		/* trap signals */
		trapsignals();
	}

	/* start the serve loop */
	if (clientserve() < 0) {
		perror("clientserve failed");
		exit(-1);
	}

	/* all's well that ends */
	exit(0);
}

RETSIGTYPE
icbexit()
{
	/* retore term modes */
	if (gv.interactive)
		restoreterm();
	/*clglog();*/
	/* close the session log */
	if (logging())
		closesessionlog();

	exit(0);
}
