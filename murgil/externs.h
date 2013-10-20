/* Copyright (c) 1988 Carrick Sean Casey. All rights reserved. */
/* For copying and distribution information, see the file "copyright.h." */

extern fd_set fdset;		/* player fd set for select() */
extern fd_set ignorefds;		/* fds that are not to be polled */
extern char *fail;		/* failcode used by logerr */
extern int port_fd;
extern int highestfd;
extern struct timeval *polltimeout;	/* timeout value in msec for select() */
extern struct itimerval *polldelay;	/* timeout value in msec for select() */
extern struct Cbuf cbufs[MAX_USERS];/* user packet buffers */
