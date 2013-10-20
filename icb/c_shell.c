/* Copyright 1990 by Carrick Sean Casey. */
/* For copying and distribution information, see the file COPYING. */

/* primitive to run a command in a shell */

#include <config.h>
#include "icb.h"
#include "externs.h"
#define _BSD
#include <sys/wait.h>
#include <signal.h>
c_shell(TARGLIST)
TARGDEFS
{
	static char *usage = "usage: c_shell [command]";
	int w, pid;
	int shellout = 0;
	int status;
	char *shell, *getenv();
	
	/* disallow use if restricted mode set */
	if (gv.restricted)
		TRETURNERR("c_shell: no shell commands in restricted mode")

	/* is this a shellout or a command? */
	if (argc == 1 || !*argv[1])
		shellout++;

	/* get shell to use */
	if ((shell = getenv("SHELL")) == NULL)
		shell = "/bin/sh";

	/* announce what we are doing */
	if (shellout) {
		sprintf(mbuf, "[%s]\r\n", shell);
		putl(mbuf, PL_SCR);
	} else {
		sprintf(mbuf, "%s", catargs(&argv[1]));
		sprintf(mbuf2, "[%s]\r\n", mbuf);
		putl(mbuf2, PL_SCR);
	}

	/* restore the terminal for the duration */
	restoreterm();

	if ((pid = fork()) == 0) {
		if (shellout)
			execlp(shell, shell, 0);
		else
			execlp(shell, shell, "-c", mbuf, 0);

		/* exec failed if we make it here */
		sprintf(mbuf, "c_shell: can't run shell \"%s\"", shell);
		putl(mbuf, PL_SL);
		putl(usage, PL_SCR);
		exit(-1);
	}

	/* trap signals that might mess ICB */
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);

	/* wait for the child(ren) to die */
	while ((w = wait(&status)) != pid && w != -1);

	/* restore the terminal */
	icbterm();

	/* restore signal processing */
	signal(SIGINT, askquit);
	signal(SIGQUIT, SIG_DFL);

	/* clean up a little */
	if (shellout)
	{
		write(1, "\r", 1);
	}

	if (gv.pauseonshell) {
		pauseprompt("[=Hit return to continue=]", 0, '\n', 0, 0);
	} else
		puts("[=returning to icb=]");
	return(TCL_OK);
}
