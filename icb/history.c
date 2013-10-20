/* Copyright (c) 1990 by Carrick Sean Casey. */
/* For copying and distribution information, see the file COPYING. */

#include <config.h>
#include "icb.h"
#include "externs.h"

/* message history routines */

static STRLIST *histhead = (STRLIST *)0; 	/* head of history list */
static STRLIST *histtail = (STRLIST *)0;	/* tail of history list */

static int histnum = 0;			/* current number of history entries */
static STRLIST *hp = (STRLIST *) 0;	/* user current location in history list */


/* add a username to the list */
/* called whenever a user sends a personal message to another */

histput(nick)
char *nick;
{
	char *malloc();
	STRLIST *sp;

	/* hunt for user within list */
	for (sp = histhead; sp; sp = sp->next)
		if (!strcasecmp(nick, sp->str)) {
			/* found user -- put at head of list */
			strunlink(sp, &histhead, &histtail);
			strlinkhead(sp, &histhead, &histtail);
			hp = histhead;
			return;
		}

	/* user wasn't found */
	if (histnum < gv.phistory) {
		/* make a new entry for the user */
		if ((sp = (STRLIST *) malloc (sizeof(STRLIST) + strlen(nick))) == NULL) {
			putl("histput: out of memory for history entries",
			 PL_SCR);
			return;
		}
		strcpy(sp->str, nick);
		strlinkhead(sp, &histhead, &histtail);
		histnum++;
	} else {
		/* history list full, link user to head, remove tail */
		sp = histtail;
		strunlink(sp, &histhead, &histtail);
		free(sp);
		histnum--;
		if ((sp = (STRLIST *)
		 malloc (sizeof(STRLIST) + strlen(nick))) == NULL) {
			putl("histput: out of memory for history entries",
			  PL_SCR);
			return;
		}
		strcpy(sp->str, nick);
		strlinkhead(sp, &histhead, &histtail);
		histnum++;
	}
	hp = histhead;
}

/* return a history entry */
/* repeatedly called, will cycle through history entries */

char *
histget()
{
	STRLIST *p;

	if (hp == 0)
		hp = histhead;

	p = hp;
	if (hp != 0)
	{
		hp = hp->next;
		if (hp == 0)
			hp = histhead;
	}
	return(p->str);
}
	

/* return number of names in current history list */

histcount()
{
	return(histnum);
}

/* delete a selected name from history list */
histdel(name)
char *name;
{
	STRLIST *p;

	for (p = histhead; p; p = p->next)
	{
		if (!strcasecmp(name,p->str))
		{
			if (hp == p)
				hp = p->next;
			strunlink(p, &histhead, &histtail);
			free(p);
			histnum--;
			break;
		}
	}
}
		
histclear()
{
	STRLIST *tmp, *p = hp;

	while (p) {
		tmp = p->next;
		free(p);
		p = tmp;
	}
	histnum = 0;
	hp = histhead = histtail = 0;
	return 0;
}

/* find a name with a given prefix - case-insensitive 
 * if unambiguous match is found, return pointer to it, set match_exact to 1,
 * and make matched nickname the top of the history 
 * if ambiguous match is found, return pointer to greatest common substring
 * of all matches and set match_exact to 0.
 * if no match is found, return NULL
 */

char *
histmatch(prefix)
char *prefix;
{
	STRLIST *p, *fp;
	int len;
	char *p1, *p2, *found;
	static char match[MAX_NICKLEN];
	
	len = strlen(prefix);
	found = NULL;
	match_exact = 0;
	
	/* printf("\rTrying to match \"%s\"\n", prefix);*/

	for (p=histhead; p; p=p->next)
	{
		 /* printf("\tTrying \"%s\":", p->str);  */
		if (!strncasecmp(prefix,p->str,len))
		{
		/*printf("Matches!\n"); */
			if (!found)
			{
				fp = p;
				strcpy(match, p->str);
				found = match;
				match_exact = 1;
			}
			else
			{
				match_exact = 0;
				for  (p1=found+len, p2=p->str+len;
						*p1 && *p2 && ccasecmp(*p1,*p2);
							++p1, ++p2)
				{
				}
				*p1 = '\0';
			}
		}
		else
		{
			/* printf("Nope.\n");  */
		}
	
	}
	if (match_exact==1)	/* we're returning a full nickname */
	{
		hp = fp->next;
		if (!hp)
			hp = histhead;
	}
	return found;
}
