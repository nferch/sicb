/* Copyright (c) 1988 Carrick Sean Casey. All rights reserved. */
/* For copying and distribution information, see the file "copyright.h." */

/* provide file descriptor set definitions for select() call */

#include <sys/param.h>

#ifndef NBBY
#define NBBY 8
#endif /* NBBY */

#ifndef	FD_SETSIZE
#define	FD_SETSIZE	(sizeof(fd_set) * NBBY)
#endif /* FD_SETSIZE */

#ifndef NFDBITS

#ifndef linux
typedef	long	fd_mask;
#endif

#define	NFDBITS	(sizeof(fd_mask) * NBBY)	/* bits per mask */
#endif /* NFDBITS */

#ifndef FD_SET
#define	FD_SET(n, p)	((p)->fds_bits[(n)/NFDBITS] |= (1 << ((n) % NFDBITS)))
#define	FD_CLR(n, p)	((p)->fds_bits[(n)/NFDBITS] &= ~(1 << ((n) % NFDBITS)))
#define	FD_ISSET(n, p)	((p)->fds_bits[(n)/NFDBITS] & (1 << ((n) % NFDBITS)))
#define	FD_ZERO(p)	bzero((char *)(p), sizeof(*(p)))
#endif /* FD_SET */


#ifndef FD_SET_SZ
#define	FD_SET_SZ(n)	(howmany((n), NFDBITS) * sizeof(fd_mask))
#endif /* FD_SET_SZ */
