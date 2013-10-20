/* Copyright (c) 1988 Carrick Sean Casey. All rights reserved. */

#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <errno.h>
#include "select.h"
#include "ipcf.h"
#include "externs.h"

/* turn off input polling for a user */

setpolltimeout(msecs)
int msecs;
{
	static struct timeval t;

	if (msecs < 0) {
		polltimeout = (struct timeval *) 0;
	} else {
		t.tv_sec = msecs / 1000;
		t.tv_usec = msecs - (t.tv_sec * 1000);
		polltimeout = &t;
	}
}

setpolldelay(msecs)
int msecs;
{
	static struct itimerval it;

	if (msecs == 0)
		polldelay = (struct itimerval *) 0;
	else {
		it.it_interval.tv_sec = 0;
		it.it_interval.tv_usec = 0;
		it.it_value.tv_sec = (int) (msecs / 1000);
		it.it_value.tv_usec =
			(msecs * 1000) - (it.it_value.tv_sec * 1000000);
		polldelay = &it;
	}
}

