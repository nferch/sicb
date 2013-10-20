/* Copyright 1990 by Carrick Sean Casey. */
/* For copying and distribution information, see the file COPYING. */

/* primitive to display the local time */

#include <config.h>
#include <string.h>
#include "icb.h"
#include "externs.h"
#include <time.h>

c_time(TARGLIST)
TARGDEFS
{
	static char *usage = "usage: c_time";

	if (argc > 1)
		TRETURNERR(usage)

	if (!strcmp(gv.timedisplay, "normal")) {
		printnormaltime();
		return(TCL_OK);
	} 

	else if (!strcmp(gv.timedisplay, "cute")) {
		printcutetime();
		return(TCL_OK);
	}
	else if (!strcmp(gv.timedisplay, "military")) {
		printmilitarytime();
		return(TCL_OK);
	}

	else {
		sprintf(TRET,
		 "c_time: bad value \"%s\" for timedisplay variable",
		 gv.timedisplay);
		return(TCL_ERROR);
	}

}


/* qtime.c	Displays time in real English, also chimes
** This version current from 18/9/89
**
** 02/90	Sean Casey 	Modified for use in forum client.
** 09/89	Ade Lovett	Complete rewrite
** 04/86	Mark Dapoz	Converted to C for UNIX
** 12/79-12/82	Mike Cowlishaw
**
*/

char *minutesaying[] = {
    "","just after ","a little after ","nearly ","almost "
};
char *fiveminsaying[] = {
    "","five past ","ten past ","a quarter past ","twenty past ",
    "twenty-five past ","half past ","twenty-five to ","twenty to ",
    "a quarter to ","ten to ","five to ",""
};
char *hoursaying[] = {
    "one","two","three","four","five","six","seven","eight","nine",
    "ten","eleven",""
};

printcutetime()
{
    char qtime[200];
    int i, hr, mn;
    struct tm *tm, *localtime();

    gettime();
    tm = localtime(&curtime);
    mn = tm->tm_min+(tm->tm_sec>29);
    hr = tm->tm_hour+(mn>32);

    strcpy(qtime, "It's ");
    strcat(qtime, minutesaying[mn%5]);
    strcat(qtime, fiveminsaying[mn/5+(mn%5>2)]);
    
    if (hr%12) {
	strcat(qtime, hoursaying[(hr -= (hr>12)*12+1)]);
	strcat(qtime, (mn%60) ? "." : " o'clock.");
    } else {
	strcat(qtime, (hr == 12) ? "Noon." : "Midnight.");
	hr = 12;
    }
    
    if (!(mn%15))
	if (mn%60) {
	     write(1, "(Ding-Dong!)\r\n\n", 15);
	}  else {
	    write(1, "(", 1);
	    for (i=hr; i>=0; i--) {
		sprintf(mbuf, "Bong%s",(i==0 ? "!)" : ","));
		putl(mbuf, PL_SCR);
	    }
	}
    sprintf(mbuf, "%s", qtime);
    putl(mbuf, PL_SCR);
}

printnormaltime()
{
	gettime();
	sprintf(mbuf, "The time is %s.", ampm(curtime, 0));
	putl(mbuf, PL_SL);
}

printmilitarytime()
{
	gettime();
	sprintf(mbuf, "The time is %s.", ampm(curtime, 1));
	putl(mbuf, PL_SL);
}
