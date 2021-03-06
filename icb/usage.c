/* Copyright (c) 1990 by Carrick Sean Casey. */
/* For copying and distribution information, see the file COPYING. */

/* routines for maintaining and displaying a usage list */
/* this list is displayed when the uses uses c_commands */

#include <config.h>
#include <stdlib.h>
#include <string.h>
#include "icb.h"
#include "externs.h"

struct Types {
	char cmdtype;
	char *description;
} types[] = {
	'g', "GENERAL COMMANDS",
	'i', "INFORMATION COMMANDS",
	'm', "MODERATOR COMMANDS",
	'c', "CUSTOM COMMANDS",
	'\0', NULL
};

USAGE *usagehead = 0, *usagetail = 0;	/* master pointers to list */

listusage(name)
char *name;
{
	USAGE *u;
	int i, p;

	for (i = 0; types[i].cmdtype != '\0'; i++) {
		for (u = usagehead, p = 0; u; u = u->next)
			if (u->type == types[i].cmdtype) {
				if (!p) {
					putl(types[i].description, PL_SCR);
					p++;
				}
				uline(u);
			}
		if (p)
			putl("", PL_SCR);
	}
}

uline(u)
USAGE *u;
{
	sprintf(mbuf, "  %-8s %-20s %s", u->name, u->args, u->usage);
	putl(mbuf, PL_SCR);
}

/* add a usage line */
/* returns zero on success, -1 on failure */

addusage(name, type, args, usage)
char *name;
char type;
char *args;
char *usage;
{
	USAGE *u;

	if ((u = (USAGE *) malloc ((unsigned) sizeof(USAGE))) == NULL)
		return(-1);

	strncpy(u->name, name, sizeof(u->name) - 1);
	u->type = type;
	strncpy(u->args, args, sizeof(u->args) - 1);
	strncpy(u->usage, usage, sizeof(u->usage) - 1);
	usagelinkin(u);
	return(0);
}


/* delete a usage line */
/* returns zero on success, -1 on failure */

deleteusage(name)
char *name;
{
}


usagelinkin(u)
USAGE *u;
{
	USAGE *i;

	/* link into head if list is empty */
	if (!usagehead)
		usagelinkhead(u, &usagehead, &usagetail);

	/* otherwise, alphabetize */
	else {
		/* find insertion point */
		for (i = usagehead; i; i = i->next) {
			if (strcmp(u->name, i->name) < 0)
				break;
		}
		if (i) {
			if (i == usagehead)
				usagelinkhead(u, &usagehead, &usagetail);
			else
				usagelinkbefore(u, i, &usagehead, &usagetail);
		} else
			usagelinktail(u, &usagehead, &usagetail);
	}
}


/* link a usage entry into the head of a list */
/* call with addresses of first and last pointers */

usagelinkhead(u, first, last)
USAGE *u, **first, **last;
{
	if (*first == 0) {
		u->prev = u->next = 0;
		*first = *last = u;
	} else {
		(*first)->prev = u;
		u->prev = 0;
		u->next = *first;
		*first = u;
	}
}

/* link a usage into the tail of a list */
/* call with addresses of first and last pointers */

usagelinktail(u, first, last)
USAGE *u, **first, **last;
{
	if (*first == 0) {
		u->prev = u->next = 0;
		*first = *last = u;
	} else {
		(*last)->next = u;
		u->prev = *last;
		u->next = 0;
		*last = u;
	}
}


/* call with addresses of first and last pointers */

gunlink(u, first, last)
USAGE *u, **first, **last;
{
	if (u->prev == 0)
		if (u->next) {
			*first = u->next;
			u->next->prev = 0;
		} else
			*last = *first = 0;
	else if (u->next == 0)
		if (u->prev) {
			*last = u->prev;
			u->prev->next = 0;
		} else
			*last = *first = 0;
	else {
		u->prev->next = u->next;
		u->next->prev = u->prev;
	}
}


/* link a usage into a list at a certain point */
/* IMPORTANT - node i must be defined */

usagelinkafter(u, i, first, last)
USAGE *u, *i, **first, **last;
{
	u->prev = i;
	u->next = i->next;
	i->next = u;
	if (i == *last)
		*last = u;
	else
		u->next->prev = u;
}

/* link a usage a list before a certain point */
/* IMPORTANT - node i and i->prev must be defined */

usagelinkbefore(u, i, first, last)
USAGE *u, *i, **first, **last;
{
	u->prev = i->prev;
	u->next = i;
	i->prev = u;
	u->prev->next = u;
}
