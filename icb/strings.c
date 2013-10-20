/* Copyright (c) 1990 by Carrick Sean Casey. */
/* For copying and distribution information, see the file COPYING. */

#include <config.h>
#include "icb.h"
#include "externs.h"
#include <varargs.h>

extern char *charmap;

#define OKGROUPCHARS	"-.!'$+,/?_"
#define OKNICKCHARS	"-.!'$+,/?_"

/* replace illegal characters in a nickname */

filternickname(txt)
char *txt;
{
/*	for (; *txt != '\0'; txt++) {
		if ((*txt >= 'A' && *txt <= 'Z') ||
		  (*txt >= 'a' && *txt <= 'z') ||
		  (*txt >= '0' && *txt <= '9'))
			continue;
		if (!index(OKNICKCHARS, *txt))
			if (*txt == ' ')
				*txt = '_';
			else
				*txt = '?';
	}*/
}


/* replace illegal characters in a regular line of text */

filtertext(char *s) {
/*	for (;*s != '\0'; s++)
		if (!(*s >= ' ' && *s < '\177'))
			*s = '_';*/
}

/* replace illegal characters from a groupname */

filtergroupname(txt)
char *txt;
{
	for (; *txt != '\0'; txt++) {
		if ((*txt >= 'A' && *txt <= 'Z') ||
		  (*txt >= 'a' && *txt <= 'z') ||
		  (*txt >= '0' && *txt <= '9'))
			continue;
		if (!index(OKGROUPCHARS, *txt))
			if (*txt == ' ')
				*txt = '_';
			else
				*txt = '?';
	}
}
/* return 1 if a string is a number */
/* else return 0 */

#include <ctype.h>

/*
numeric(txt)
char *txt;
{
	for (;*txt != '\0'; txt++)
		if (!(*txt >= '0' && *txt <= '9'))
			return(0);
	return(1);
}
*/

#define UC(x) ((x >= 'a' && x <= 'z') ? x & ~040 : x)

strcicmp(s1, s2)
char *s1, *s2;
{
	while (UC(*s1) == UC(*s2)) {
		s2++;
		if (*s1++ == '\0')
			return(0);
	}
	return(UC(*s1) - UC(*s2));
}

strcincmp(s1, s2, n)
char *s1, *s2;
int n;
{
	while (--n >= 0 && UC(*s1) == UC(*s2)) {
		s2++;
		if (*s1++=='\0')
			return(0);
	}
	return((n < 0) ? 0 : (UC(*s1) - UC(*s2)));
}

char *
findspace(s)
char *s;
{
	/* find EOS or whitespace */
	while (*s != '\0' && *s != ' ' && *s != '\t') s++;

	if (*s == '\0')
		/* oops found no whitespace */
		return(0);
	else {
		/* point to char *after* whitespace */
		*s = '\0';
		return(++s);
	}
}

/* convert a string to lower case */
lcaseit(s)
char *s;
{
	for (; *s; s++)
		if (*s >= 'A' && *s <= 'Z')
			 *s |= 040;
}

/* return how many characters in string1 matched string2 */
cimatch(s1, s2)
char *s1, *s2;
{
	int count = 0;

	for (; *s1 && *s2 && (*s1 == *s2); s1++, s2++)
		count++;
	return(count);

}

/* put a string in quotes */
/* puts backslashes before all special chars appearing in the string */
/* doesn't interfere with already backslashed chars */

char *special = "{}[]\";$\\";

quoteify(a, b)
char *a, *b;
{
#ifdef NEVER
	while ((*a == ' ' || *a == '\t') && *a != '\0') *b++ = *a++;
	while (*a != ' ' && *a != '\t' && *a != '\0') *b++ = *a++;
	while ((*a == ' ' || *a == '\t') && *a != '\0') *b++ = *a++;
#endif /* NEVER */

	while (*a != '\0') {
		if (index(special, *a)) {
			*b++ = '\\';
			*b++ = *a;
		} else
			*b++ = *a;
		a++;
	}
	*b = '\0';
}


static char *catargsbuf = NULL;

char *
catargs(argv)
char **argv;
{
	char *s, **ap, *p;
	int len = 0;

	for (ap = argv; *ap; ++ap)
	{
		len+=strlen(*ap)+1;
	}
	
	if (catargsbuf)
		free(catargsbuf);

	if ((catargsbuf=(char *)malloc(len+1))==NULL)
	{
		fprintf(stderr,"Out of memory in catargs().\n");
		icbexit();
	}

	p = catargsbuf;
	while (*argv != NULL) {
		s = *argv;
		while (*s) *p++ = *s++;
		if (*(argv+1) != NULL)
			*p++ = ' ';
		argv++;
	}
	*p = '\0';
	return catargsbuf;
}

/* this is a bad hack! */

mbreakprint(int per, char *from, char *s) {
	int count = 0;
	int printedlines=0;
	char *p, *lastw;
	char tmp1, tmp2;

	/* traverse s and try to break on a word */
	p = s;
	lastw = p;
	while (*p != '\0') {
		if (*p == '\n') {
			*p++ = '\0';
			mprint(per, from, s, 1);
			count = 0;
			lastw = p;
			s = p;
			continue;
		}
		if (*p == ' ')
			/* remember location of last word break */
			lastw = p;

		/* break line if we are at max length */
		if (count == (MAX_TEXTLEN - 1)) {
			if ((p - lastw) > 40) {
				/* have to break in the middle of a word */
				tmp1 = *(p - 1);
				tmp2 = *p;
				*(p - 1) = '-';
				*p = '\0';
				if (printedlines==0)
					mprint(per, from, s,1);
				else 
					mprint(per, from, s,0);
				printedlines++;

				*(p - 1) = tmp1;
				*p = tmp2;
				p--;
				s = p;
			} else {
				/* break at last space char */
				tmp1 = *lastw;
				*lastw = '\0';
				if (printedlines==0)
					mprint(per, from, s,1);
				else 
					mprint(per, from, s,0);
				printedlines++;
				*lastw = tmp1;
				p = lastw + 1;
				s = p;
			}
			lastw = p;
			count = 0;
		} else {
			count++;
			p++;
		}
	}
	if (count > 0) {
		if (printedlines==0)
			mprint(per, from, s,1);
		else 
			mprint(per, from, s,0);
		printedlines++;
	}
}

mprint(int per, char *from, char *s, int show) {
	char blanknick[20];
	int blah=0;
	char *realnick;
	char geetee;
	strcpy(blanknick,from);
	for (blah;blanknick[blah]!='\0';blah++){
		blanknick[blah]=' ';
	}
	if (show==0) {
		geetee=' ';
		realnick=blanknick;
	}
	else {
		geetee=PROMPTKAR;
		realnick=from;
	}
	fflush (stdout);
	if (per) {
		if (show) { 
			if (per == 2) {
				sprintf(mbuf, "%s<[%s%s%s]%c%s %s%s",cgtlt,cwarning,realnick,cgtlt,geetee,clogintime,s,csane);
			} else {
				sprintf(mbuf, "%s<*%s%s%s*%c%s %s%s",cgtlt,cwarning,realnick,cgtlt,geetee,clogintime,s,csane);
			}
		} else {
			if (per == 2) {
				sprintf(mbuf, "%s  %s%s%s]%c%s %s%s",cgtlt,cwarning,realnick,cgtlt,PROMPTKAR,clogintime,s,csane);
			} else {
				sprintf(mbuf, "%s  %s%s%s*%c%s %s%s",cgtlt,cwarning,realnick,cgtlt,PROMPTKAR,clogintime,s,csane);
			}
		}
		putl(mbuf, PL_BUF | PL_SCR );
		sprintf(mbuf, "<*%s*%c %s",realnick,PROMPTKAR,s);
		putl(mbuf, PL_LOG | PL_GLOG);
	} else{
		sprintf(mbuf, "%s%s%s%c%s %s%s",cnormal,realnick,cgtlt,geetee,cwhisper,s,csane);
		putl(mbuf, PL_BUF | PL_SCR);
		sprintf(mbuf, "%s%c %s",realnick,geetee,s);
		putl(mbuf, PL_LOG);
	}
}

wordcmp(s1, s2)
char *s1, *s2;
{
	while(*s1 == *s2++)
		if (*s1 == '\0' || *s1 == ' ' || *s1++ == '\t')
			return(0);
	if (*s1 == ' ' && *(s2-1) == '\0')
		return(0);
	return(*s1 - *--s2);
}

wordcasecmp(s1, s2)
char *s1, *s2;
{
	char *cm;
	while(cm[*s1] == cm[*s2++])
		if (*s1 == '\0' || *s1 == ' ' || *s1++ == '\t')
			return(0);
	if (*s1 == ' ' && *(s2-1) == '\0')
		return(0);
	return(cm[*s1] - cm[*--s2]);
}

char *
getword(s)
char *s;
{
	static char word[64];
	char *w = word;
	while (*s != ' ' && *s != '\t' && *s != '\0' && ((w - word) < 64))
		*w++ = *s++;
	*w = '\0';
	return(word);
}

/* Read a line containing zero or more colons. Split the string into */
/* an array of strings, and return the number of fields found. */

char *fields[MAX_FIELDS];

int split(char *s) {
	char *p = s;
	int i = 0;

	fields[i] = s;
	for(;;) {

		i++;

		/* find delim or EOS */
		while(*p != '\001' && *p != '\0') p++;

		if (*p == '\001') {
			/* got delim */	
			*p = '\0';
			fields[i] = ++p;
		} else
			return(i);
	}
}

#if 0

/* cat multiple strings into one */
/* call with strscat(dest, src, src, ...) */

#define va_dcl butterfucker
char *
strscat(va_dcl) {
	char *dest, *src;
	va_list args;

	va_start(args);

	/* get the dest pointer */
	if ((dest = va_arg(args, char *)) == 0) {
		puts("fatal error: strscat called without dest");
		exit(-1);
	}

	/* copy strings */
	while ((src = va_arg(args, char *)) != NULL) {
		while (*src != '\0')
			*dest++ = *src++;
	}

	*dest = '\0';
	return(dest);
}

#endif

char *basename(path)
char *path;
{
char *comp;

comp=path;
while (*path)
{
	if (*path++=='/')
		comp=path;
}

return comp;
}













