/* Copyright (c) 1990 by Carrick Sean Casey. */
/* For copying and distribution information, see the file COPYING. */

/* primitive to control the session logging function */

#include <errno.h>
#include <config.h>
#include "icb.h"
#include "externs.h"
#include <time.h>

FILE *logfp = NULL;

c_log(TARGLIST)
TARGDEFS
{
	/* disallow use in restricted mode */
	if (gv.restricted)
		TRETURNERR("c_log: logging not allowed in restricted mode")

	if (argc == 1 || !argv[1]) {
		if (logging())
			closesessionlog();
		else
			if (startsessionlog(interp, 0) < 0)
				return(TCL_ERROR);
	} else {
		if (logging()) {
			sprintf(TRET, "c_log: session logging already on");
			return(TCL_ERROR);
		} else
			if (startsessionlog(interp, argv[1]) < 0)
				return(TCL_ERROR);
	}
	return(TCL_OK);

}


/* start a session log */

startsessionlog(interp, path)
Tcl_Interp *interp;
char *path;
{
	char *aptr, *ampm();
	time_t time();
	struct tm *t, *localtime();
	time_t clock;
#if !defined (__GNU_LIBRARY__) && !defined (STDC_HEADERS)
	extern int errno;
#endif
	extern int sys_nerr;

	/* determine pathname to use */
	if (path == NULL || *path == '\0')
		if (gv.logfile)
			path = gv.logfile;
		else
			path = "icb.log";

	/* expand a tilde style path */
	if (*path == '~')
		if ((path = tildexpand(path)) == NULL) {
			strcpy(TRET, "c_log: bad login id in path");
			return(-1);
		}
	
	/* open the session log */
	if ((logfp = fopen(path, "a")) == NULL) {
		if (errno > sys_nerr)
		   sprintf(TRET,
		    "c_log: can't open \"%s\": errno %d", path, errno);
		else
		   sprintf(TRET,
		    "c_log: can't open \"%s\": %s", path, sys_errlist[errno]);
		return(-1);
	}

	/* protect the logfile against others */
	if (fchmod((int)(fileno(logfp)), 0600) != 0) {
		if (errno > sys_nerr)
		   sprintf(TRET,
		    "c_log: can't fchmod \"%s\": errno %d", path, errno);
		else
		   sprintf(TRET,
		    "c_log: can't fchmod \"%s\": %s", path, sys_errlist[errno]);
		fclose(logfp);
		logfp = NULL;
		return(-1);
	}

	/* line buffer it */
	linebuffer(logfp);

	/* get the time so we can timestamp it */
	time(&clock);
	t = localtime(&clock);
	aptr = ampm(clock, 0);
	if (*aptr == ' ')
		aptr++;
	
	/* timestamp it */
	sprintf(mbuf, "Session log \"%s\" started at %d/%02d/%-2d %s.",
	  path, t->tm_mon+1, t->tm_mday, t->tm_year, aptr);
	putl(mbuf, PL_SCR | PL_LOG);

	return(0);
}


/* close the logfile */

closesessionlog()
{
	char *aptr, *ampm();
	time_t time();
	struct tm *t, *localtime();
	time_t clock;

	/* do nothing if we're not really logging */
	if (logfp == NULL)
		return;

	/* get the time so we can timestamp it (deja vu?) */
	time(&clock);
	t = localtime(&clock);
	aptr = ampm(clock, 0);
	if (*aptr == ' ')
		aptr++;
	
	/* timestamp it */
	sprintf(mbuf, "Session log closed at %d/%02d/%-2d %s.",
	  t->tm_mon+1, t->tm_mday, t->tm_year, aptr);
	putl(mbuf, PL_SCR | PL_LOG);

	/* close it */
	fclose(logfp);
	logfp = NULL;
	return;
}


/* return 1 if session logging is in effect, otherwise return 0 */

logging()
{
	if (logfp == NULL)
		return(0);
	return(1);
}


