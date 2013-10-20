/* Copyright 1990 by Carrick Sean Casey. */
/* For copying and distribution information, see the file COPYING. */

/* primitive to generate a random number */

#include <config.h>
#include <string.h>
#include "icb.h"
#include "externs.h"

c_rand(TARGLIST)
TARGDEFS
{
	static char *usage = "usage: c_rand number";
	int range;
	char num[64];

	if (argc != 2 || !*argv[1])
		TRETURNERR(usage)

	if ((range = atoi(argv[1])) == 0)
		TRETURNERR("c_rand: zero or non-numeric argument given")

	sprintf(num, "%d", (rand() % range) + 1);

	/* pass the number back to the caller */
	Tcl_Return(interp, num, TCL_VOLATILE);
	
	return(TCL_OK);
}
