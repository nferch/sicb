/* Copyright (c) 1988 Carrick Sean Casey. All rights reserved. */

#include <sys/types.h>
#include "ipcf.h"

struct Cbuf cbufs[MAX_USERS+1];/* user packet buffers (YECCH!) */
fd_set fdset;			/* player fd set for select() */
fd_set ignorefds;			/* fds that are not to be polled */
int port_fd;
int highestfd = 0;
char *fail;
/* timeout value in usec for select() */
struct timeval *polltimeout = (struct timeval *)0;
struct itimerval *polldelay = (struct itimerval *)0;
