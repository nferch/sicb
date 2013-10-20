/* Copyright (c) 1990 by Carrick Sean Casey. */
/* For copying and distribution information, see the file COPYING. */

/* TCL interface */

#include <config.h>
#ifdef linux
#include <unistd.h>
#include <sys/dir.h>
#else
#include <dirent.h>
#endif

#include "icb.h"
#include "externs.h"
#include <sys/file.h>

Tcl_Interp *interp;

/* function definitions */
int oset();
int ounset();
int c_hclear();
int c_hdel();
int c_help();
int c_hush();
int c_log();
int c_quit();
int c_rand();
int c_replay();
int c_shell();
int c_time();
int c_usage();
int c_version();
int s_admin();
int s_beep();
int s_bump();
int s_dh();
int s_chpw();
int s_motd();
int s_group();
int s_open();
int s_personal();
int s_register();
int s_send();
int s_spersonal();
int s_user();
int s_version();
int s_who();

/* initialize TCL */
tclinit()
{
	char *p;
	extern char *defprocs[];
	int x, n;

	/* initialize the interpreter */
	interp = Tcl_CreateInterp();

	/* add the client primitives */
	Tcl_CreateCommand(interp, "oset", (Tcl_CmdProc *)oset, (ClientData) 0, NULL);
	Tcl_CreateCommand(interp, "ounset", (Tcl_CmdProc *)ounset, (ClientData) 0, NULL);
	Tcl_CreateCommand(interp, "c_log", (Tcl_CmdProc *)c_log, (ClientData) 0, NULL);
	Tcl_CreateCommand(interp, "c_hclear", (Tcl_CmdProc *)c_hclear, (ClientData) 0, NULL);
	Tcl_CreateCommand(interp, "c_hdel", (Tcl_CmdProc *)c_hdel, (ClientData) 0, NULL);
	Tcl_CreateCommand(interp, "c_help", (Tcl_CmdProc *)c_help, (ClientData) 0, NULL);
	Tcl_CreateCommand(interp, "c_hush", (Tcl_CmdProc *)c_hush, (ClientData) 0, NULL);
	Tcl_CreateCommand(interp, "c_quit", (Tcl_CmdProc *)c_quit, (ClientData) 0, NULL);
	Tcl_CreateCommand(interp, "c_rand", (Tcl_CmdProc *)c_rand, (ClientData) 0, NULL);
	Tcl_CreateCommand(interp, "c_replay", (Tcl_CmdProc *)c_replay, (ClientData) 0, NULL);
	Tcl_CreateCommand(interp, "c_shell", (Tcl_CmdProc *)c_shell, (ClientData) 0, NULL);
	Tcl_CreateCommand(interp, "c_time", (Tcl_CmdProc *)c_time, (ClientData) 0, NULL);
	Tcl_CreateCommand(interp, "c_usage", (Tcl_CmdProc *)c_usage, (ClientData) 0, NULL);
	Tcl_CreateCommand(interp, "c_version", (Tcl_CmdProc *)c_version, (ClientData) 0, NULL);
	Tcl_CreateCommand(interp, "s_admin", (Tcl_CmdProc *)s_admin, (ClientData) 0, NULL);
	Tcl_CreateCommand(interp, "s_beep", (Tcl_CmdProc *)s_beep, (ClientData) 0, NULL);
	Tcl_CreateCommand(interp, "s_bump", (Tcl_CmdProc *)s_bump, (ClientData) 0, NULL);
	Tcl_CreateCommand(interp, "s_chpw", (Tcl_CmdProc *)s_chpw, (ClientData) 0, NULL);
	Tcl_CreateCommand(interp, "s_dh", (Tcl_CmdProc *)s_dh, (ClientData) 0, NULL);
	Tcl_CreateCommand(interp, "s_motd", (Tcl_CmdProc *)s_motd, (ClientData) 0, NULL);
	Tcl_CreateCommand(interp, "s_group", (Tcl_CmdProc *)s_group, (ClientData) 0, NULL);
	Tcl_CreateCommand(interp, "s_open", (Tcl_CmdProc *)s_open, (ClientData) 0, NULL);
	Tcl_CreateCommand(interp, "s_personal", (Tcl_CmdProc *)s_personal, (ClientData) 0, NULL);
	Tcl_CreateCommand(interp, "s_register", (Tcl_CmdProc *)s_register, (ClientData) 0, NULL);
	Tcl_CreateCommand(interp, "s_send", (Tcl_CmdProc *)s_send, (ClientData) 0, NULL);
	Tcl_CreateCommand(interp, "s_spersonal", (Tcl_CmdProc *)s_spersonal, (ClientData) 0, NULL);
	Tcl_CreateCommand(interp, "s_user", (Tcl_CmdProc *)s_user, (ClientData) 0, NULL);
	Tcl_CreateCommand(interp, "s_version", (Tcl_CmdProc *)s_version, (ClientData) 0, NULL);
	Tcl_CreateCommand(interp, "s_who", (Tcl_CmdProc *)s_who, (ClientData) 0, NULL);

	/* set up the default bindings */
	for (x = 0; defprocs[x] != 0; x++)
		if (Tcl_Eval(interp, defprocs[x]) != TCL_OK) {
			printf("Default binding failed: %s\r\n", defprocs[x]);
			if (*interp->result != 0)
				puts(interp->result);
		}

}

tclreadicbrc()
{
	char *p, *path;
	char cmd[MAXNAMLEN+10];
	
	if (!(path = getrcname())) return;

	sprintf(cmd, "source %s", path);
	if (Tcl_Eval(interp, cmd) != TCL_OK) {
		printf("Error in rc file \"%s\".\r\n", path);
		if (*interp->result != 0)
			puts(interp->result);
	}
}


/* find the pathname of the rc file to read */
/* ICBRC or FNRC optionally contains the pathname of the rc file */
/* if the file exists, then an error will be printed if it can't be read */

char *
getrcname()
{
	static char *names[] = { ".icbrc", ".fnrc", ".tclrc" , (char *)NULL };
	char *rcfile, *homedir, *getenv(), **p;
	static char rcpath[MAXNAMLEN+1];	/* path of startup file */

	/* get name of file to use */
	if (((rcfile = getenv("ICBRC")) != NULL)
	 || ((rcfile = getenv("FNRC")) != NULL))
	{
		if (*rcfile != '/') {
			/* relative to HOME */
			if ((homedir = getenv("HOME")) == NULL) {
			      printf("icb: $HOME not set - can't read \"%s\"\r\n.",
				rcfile);
			      return(NULL);
			}
			sprintf(rcpath, "%s/%s", homedir, rcfile);
			rcfile = rcpath;
		}

		/* make sure the file is there */
		if (access(rcfile, (F_OK | R_OK)) < 0) {
			printf("icb: can't read rc file \"%s\".\r\n", rcfile);
			return(NULL);
		}

		return rcfile;
	} 

	if ((homedir = getenv("HOME")) == NULL) {
		printf("icb: warning - $HOME not set.\r\n");
		return(0);
	}

	for (p=names; p && *p; ++p)
	{
		sprintf(rcpath, "%s/%s", homedir, *p);
		rcfile = rcpath;
		if (access(rcfile, R_OK)==0)
			break;
	}
	if (p)
		return rcfile;

	return(0);
}

tclrestrict()
{
	/* delete commands that might shellout or read or write files */
	Tcl_DeleteCommand(interp, "exec");
	Tcl_DeleteCommand(interp, "print");
	Tcl_DeleteCommand(interp, "file");
	Tcl_DeleteCommand(interp, "source");
}
