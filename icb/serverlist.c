#include <config.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "icb.h"

/*
 * serverlist.c - management routines for the server list
 */

static struct server serverbuf;

static FILE *serverfile = NULL;

FILE *openserverfile()
{
	struct stat statbuf;
	char *personalsl;
	char command[256];
	char pwd[256];
	FILE *ret;

#ifndef HAVE_CWD
	getwd(pwd);
#else /* SYSV */
	getcwd(pwd, MAXPATHLEN+1);
#endif /* SYSV */
	chdir((char *)getenv("HOME"));
	if (!stat(PERSONALSL,&statbuf))
	{
		sprintf(command,"/bin/cat %s %s\n", PERSONALSL, ICBSERVERLIST);
		ret= popen(command,"r");
	}
	else
		ret= fopen(ICBSERVERLIST,"r");
	chdir(pwd);
	return ret;
}


struct server *getserver()
{
static int def = 0;

	if (serverfile == NULL)
	{
		if (!(serverfile=openserverfile()))
		{
			if (!def)
			{
				def=1;
				strcpy(serverbuf.host, ICBSERVER);
				serverbuf.port = ICBPORT;
				return &serverbuf;
			}
			else
				return NULL;
		}
	}


	if (!readserverent(serverfile, &serverbuf))
	{	
		fclose(serverfile);
		serverfile = NULL;
		return NULL;
	}

	return &serverbuf;
}

struct server *getservbyname(name)
char *name;
{
	if (!serverfile)
		if (!(serverfile=openserverfile()))
			return NULL;
	else
		rewind(serverfile);

	while (readserverent(serverfile, &serverbuf))
	{
		if (!strcmp(name, serverbuf.name))
			return &serverbuf;
	}
	fclose(serverfile);
	serverfile = NULL;
	return NULL;
}

void listservers()
{
	if (!serverfile)
		if (!(serverfile=openserverfile()))
			return;
	else
		rewind(serverfile);

	printf("%-16s%-32s%5s\n", "SERVER", "HOST", "PORT");
	while (readserverent(serverfile, &serverbuf))
	{
		printf("%-16s%-32s%5d\n", serverbuf.name, serverbuf.host,
serverbuf.port);
	}
}

int readserverent(fd,buffer)
FILE *fd;
struct server *buffer;
{
char line[256];
int index;

for (;;)
{
if (!fgets(line,256,fd))
{
	return 0;
}

index = 0;

while (line[index] && isspace(line[index]))
	++index;

if (!line[index] || line[index] == '#')
{
	continue;
}
	
if ((sscanf(line, "\"%[^\"]\" %s %d", buffer->name, buffer->host,
	&(buffer->port))!=3) &&
	(sscanf(line, "%s %s %d", buffer->name, buffer->host,&(buffer->port))!=3))
{
	buffer->port = ICBPORT;
	if ((sscanf(line, "\"%[^\"]\" %s", buffer->name, buffer->host)!=2) &&
	    (sscanf(line, "%s %s", buffer->name, buffer->host)!=2))
	{
		if (sscanf(line, "%s", buffer->host) == 1)
		{
			strcpy(buffer->name, buffer->host);
		}
		else
		{
			fprintf(stderr,"Bad server entry: %sSkipping...\n",line);
			continue;
		}
	}
}

return 1;
}
}
