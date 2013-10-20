/* Copyright (c) 1990 by Carrick Sean Casey. */
/* For copying and distribution information, see the file COPYING. */

/* screen and logfile printing routines */

#include <config.h>
#include <string.h>
#include "icb.h"
#include "externs.h"
extern FILE *glogfp;
extern FILE *logfp;

/* write to the screen, and maybe to the logfile and review buffer */

putl(s, flags)
char *s;
int flags;
{
	char printbuf[1024];
	char *p = printbuf;
	char *t = s;

	if (continued) {
		linenumber = 0;
		continued = 0;
	}

	if (flags & PL_SCR) {
		/* if paging in effect, do paging */
		if (gv.pagesize && (++linenumber >= gv.pagesize)) {
			sprintf(cgenbuff,"%s[=%sMore%s=]%s",cequalbracket,cmore,cequalbracket,csane);
			pauseprompt(cgenbuff, 1, 0, 1,  " ");
			linenumber = 0;
		}

		/* write to the screen */
		while (*t != '\0')
			*p++ = *t++;
		*p++ = '\r';
		*p++ = '\n';
		write(1, printbuf, p - printbuf);
	}
	/* put line into session log */
	if ((flags & PL_LOG) && logfp != NULL) {
		fprintf(logfp, "%s", s);
		putc('\n', logfp); 
	}
	
	/* add to review buffer */
	if ((flags & PL_BUF) && gv.bufferlines)
		bufferadd(s);
}


/* print a prompt, and wait for a character */
/* if Erase is nonzero, the prompt is Erased by backspacing */
/* if c is non-null, use must type c to continue */
/* if unget it set, user's character is pushed back into input buffer, unless
   it is one of the characters in except. */

pauseprompt(prompt, Erase, c, unget, except)
char *prompt;
int Erase;
char c;
int unget;
char *except;
{
	char uc, *p;

	/* print the prompt */
	write(1, prompt, strlen(prompt));

	if (c != '\0')
		while ((uc = getchar()) != c);
	else
		uc = getchar();

	/* Erase the prompt if requested */
	if (Erase) {
		char Erasebuf[512];
		char *m = Erasebuf;

		for (p = prompt; *p != '\0'; p++) {
			*m++ = '\b';
			*m++ = ' ';
			*m++ = '\b';
		}
		write(1, Erasebuf, m - Erasebuf);
	} else
		write(1, "\r\n", 2);

	/* push character back onto stream if requested */
	if (unget)
		if (!except || !index(except,uc))
			pushback(uc);
}
