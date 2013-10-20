/* Copyright (c) 1990 by Carrick Sean Casey. */
/* For copying and distribution information, see the file COPYING. */

/* send various messages to the server */

#include <config.h>
#include "icb.h"
#include "externs.h"
#include <icbcrypt.h>

char **msplit();
char *findspace();

/* send normal open group message to the server */

csendopen(txt)
char *txt;
{
	char **p;
	int i;

	p = msplit(txt, MAX_INPUTSTR);
	if (!p)
	{
		sprintf(cgenbuff,"%s[=%sError%s=]%s Out of memory for splitting packet.%s",cequalbracket,cerror,cequalbracket,cwarning,csane);
		putl(cgenbuff,PL_SCR);
		return 0;
	}

	for (i=0; p[i]; ++i)
	{
		sprintf(pbuf,"%c",M_OPEN);
		strcat(pbuf,p[i]);
		setlen;
		sendit(pp);
		strcpy(mbuf,p[i]);
		putl(mbuf, PL_BUF | PL_LOG);
	}

}


/* send a login packet */

sendlogin(id, nick, group, command, passwd)
char *id, *nick, *group, *command, *passwd;
{

	sprintf(pbuf, "%c%s\001%s\001%s\001%s\001%s",
	  M_LOGIN, id, nick, group, command, passwd);
	setlen;
	sendit(pp);
}

/* send a pong */
sendpong()
{
	*pbuf = M_PONG;
	*pp = 1;
	sendit(pp);
}


/* send a command to the server */

sendcmd(cmd, args)
char *cmd, *args;
{
	char *nick;
	char **p;
	int i;
	int maxlen;
	int enc = 0;
	char * encryptedmessage;

	if (strlen(cmd) > MAX_INPUTSTR - 5 )
	{
		fprintf(stderr,"%s[=%sError%s=]%s Command too long.\n%s",cequalbracket,cerror,cequalbracket,cwarning,csane);
		return 1;
	}

	if (!args)
	{
		sprintf(pbuf, "%c%s\001", M_COMMAND, cmd);
		setlen;
		sendit(pp);
		return 0;
	}
		
	maxlen = MAX_INPUTSTR - strlen(cmd) - 1;

	/* special case commands that take a nickname */
	if ((strcasecmp(cmd, "write")==0) || (strcasecmp(cmd, "m")==0) || (strcasecmp(cmd, "menc")==0))
	{
		nick=args;
		args=findspace(args);
		if (!strcasecmp(cmd,"menc")) {
			enc = 1;
		}
		maxlen = maxlen - strlen(nick) - 1;
		if (enc) {
			maxlen = maxlen * 0.75;
		}
			
	}
	else
		nick = NULL;

	p = msplit(args, maxlen);
	if (!p)
	{
		sprintf(cgenbuff,"%s[=%sError%s=]%s Out of memory to split packet%s",cequalbracket,cerror,cequalbracket,cgenmessage,csane);
		putl(cgenbuff, PL_SCR);
		return 0;
	}

	for (i=0; p[i]; ++i)
	{
		sprintf(pbuf,"%c%s\001",M_COMMAND, (enc ? "m" : cmd));
		if (nick)
		{
			strcat(pbuf, nick);
			strcat(pbuf, " ");
		}
		if (enc) {
			encryptedmessage = blowfishEncrypt(nick, p[i], strlen(p[i]));
			if (encryptedmessage != NULL) {
				strcat(pbuf,"{!BF:");
				strcat(pbuf,encryptedmessage);
				strcat(pbuf,"}");
				free(encryptedmessage);
			} else {
				printf("Cannot encrypt to %s. Have you completed a key agreement?\n",nick);
			}
		} else {
			strcat(pbuf,p[i]);
		}
		setlen;
		sendit(pp);
	}
}


static char **chunks;


/*
 * split a message up into smaller chunks
 */
char **msplit(s,size)
char *s;
int size;
{
int nelem;
int i;
int length;
char ch;

if (chunks)
{
	for (i=0; chunks[i]; ++i) {
		free(chunks[i]);
	}
	free(chunks);
}

if (!s)
{
	chunks = (char **)calloc(nelem, sizeof(char *));
	chunks[0] = (char *) 0;
	return chunks;
}

nelem = strlen(s)/size+2;
chunks = (char **)calloc(nelem, sizeof(char *));

for ( i = 0 ; strlen(s) > size ; ++i)
{
	length = size;
	while (!isspace(s[length-1])&&length>1)
		length--;

	ch='\0';
	if (length <= 1)
	{
		length=size;
		ch = s[length-1];
		s[length-1] = '-';
	}
			
	chunks[i] = (char *)malloc(length + 1);
	if (!chunks[i])
	{
		sprintf(cgenbuff,"%s[=%sError%s=]%s Out of memory%s", cequalbracket,cerror,cequalbracket,cgenmessage,csane);
		putl(cgenbuff, PL_SCR);
		return (char **) 0;
	}
	strncpy(chunks[i],s,length);
	chunks[i][length]='\0';
	s+=length;
	if (ch)
		*(--s) = ch;
}
chunks[i] = (char *)malloc(strlen(s)+1);
if (!chunks[i])
{
	sprintf(cgenbuff,"%s[=%sError%s=]%s Out of memory%s", cequalbracket,cerror,cequalbracket,cgenmessage,csane);
	putl(cgenbuff, PL_SCR);
	return (char **) 0;
}
strcpy(chunks[i],s);
chunks[++i] = (char *) 0;
return chunks;
}
