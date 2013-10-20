/* Copyright (c) 1988 Carrick Sean Casey. All rights reserved. */
/* For copying and distribution information, see the file "copyright.h." */

#include <sys/param.h>

#define MAX_USERS	(NOFILE - 5)
#define USER_BUF_SIZE	1024
#define WARN		1
#define FATAL		2

/* packet input buffer */

struct Cbuf {
	char buf[USER_BUF_SIZE];
	char *rptr;	/* pointer to location for next read */
	char new;	/* set to 1 if next read is start of new data packet */
	unsigned char size;
	unsigned char remain;	/* packet characters remaining to be read */
};
