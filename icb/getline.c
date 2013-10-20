#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include "icb.h"
#include "externs.h"

#ifdef HAVE_READLINE_H
#include <readline.h>
#endif

#ifdef HAVE_READLINE_READLINE_H
#include <readline/readline.h>
#endif

/* get a line from the user, with input processing */
/* returns chars in line, or 0 if user erased to beginning of line */

/* A static variable for holding the line. */
static char *line_read = (char *)NULL;

extern void echo();

char *icb_getline(prompt, expand)
char *prompt;
int expand;
{

	int ch;
	linenumber = 0;
       /* If the buffer has already been allocated, return the memory
          to the free pool. */
       if (line_read != (char *)NULL)
         {
           free (line_read);
           line_read = (char *)NULL;
         }
	
       /* Get a line from the user. */
       echo();
       line_read = readline (prompt);
       noecho();
     
       /* If the line has any text in it, save it on the history. */
       if (line_read && *line_read)
         	add_history (line_read);

	if (!line_read) /* EOF found */
		putl("",PL_SCR);

	return line_read;
}
     
/*
 *  keystroke handler for tab key
 */
int
handletab(count, c)
int count;
char c;
{
	char mpref[256];
	int ppoint;
	int words;
	int term;
	int i;
	int word2, word2len, diff;
	char find_nick[MAX_NICKLEN];
	char *found_nick;
	char *histmatch();
	char msgcmdchar;
	
	/* make sure the history's not empty - punt if it is */
	if (histcount()==0)
	{
		return;
	}

	/* remember cursor location */
	ppoint = rl_point;
	
	/* determine case */
	if (rl_end==0 || rl_line_buffer[0] != gv.cmdchar)
	{
		/* case 1 - no command char */
		if (found_nick = histget())
		{
			if (getNickContext(found_nick,0)) {
				msgcmdchar = 'n';
			} else {
				msgcmdchar = 'm';
			}
			sprintf(mpref,"%c%c",gv.cmdchar,msgcmdchar);
			strcat(mpref, " ");
			strcat(mpref, found_nick);
			strcat(mpref, " ");
			rl_point = 0;
			rl_insert_text(mpref);
			rl_point = rl_end;
			/* rl_redraw(); */
			rl_redisplay();
		}
		else
			printf("\007");
		return;
	}
	
	/* starts with command character */
	words=0;
	term=0;
	word2len=-1;
	if (rl_end)
	{
		words++;
		for (i=0; i<rl_end; ++i)
		{
			if (isspace(rl_line_buffer[i]))
			{
				term++;
				if (words==2)
					word2len=i-word2;
			}
			else if (term)
			{
				term=0;
				words++;
				if (words==2)
					word2=i;
					
			}
		}
		if (word2len<0)
			word2len = rl_end - word2;
	}
	
	/* case 3 - add nickname */
	if ((words==1) && term && rl_point == rl_end)
	{
		if (found_nick=histget())
		{
			rl_insert_text(histget());
			rl_insert(1,' ');
			rl_point = 1;
			rl_delete_text(1,rl_end);
			/*if (getNickContext(found_nick,0)) {
				rl_insert(1,'n');
			} else {
				rl_insert(1,'m');
			}
			*/
			rl_point = rl_end;
			/* rl_redraw(); */
			rl_refresh_line(1,rl_end);
		}
		else
			printf("\007");
		return;
	}
	
	/* case 4 - nickname completion */
	if ((words==2) && !term && rl_point == rl_end)
	{
		if (word2len > MAX_NICKLEN)
		{
			printf("\007");
			return;
		}
		
		if (rl_line_buffer[word2]=='@')
		{
			word2++;
			word2len--;
		}

		for (i=word2; i<rl_end; ++i)
		{
			find_nick[i-word2]=rl_line_buffer[i];
		}
		find_nick[i-word2]='\0';
		found_nick = histmatch(find_nick);
		if (found_nick)
		{
			rl_delete_text(word2,word2 + word2len);
			rl_point = word2;
			rl_insert_text(found_nick);
			if (match_exact)
				rl_insert(1, ' ');
			else
				printf("\007");
			rl_redisplay();
		}
		else
			printf("\007");
		return;
	}
	
	/* case 5 - replace nickname */
	if (words >= 2)
	{
		found_nick = histget();
		if (found_nick)
		{
			//printf("word2 is %d and word2len is %d and found_nick is %s\n",word2,word2len,found_nick);
			rl_point = word2;  
			rl_delete_text(rl_point,rl_point + word2len); 
			rl_insert_text(found_nick);
			rl_redisplay();
			rl_delete_text(1,2);
			rl_point = 1;
			if (getNickContext(found_nick,0)) {
				rl_insert_text("n");
			} else {
				rl_insert_text("m");
			}
			rl_point = rl_end;
			rl_redisplay();
		}
		else
			printf("\007");
		return;
	}
		
	/* anything else */
	printf("\007");
}
