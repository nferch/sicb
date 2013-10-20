/* Copyright (c) 1990 by Carrick Sean Casey. */
/* For copying and distribution information, see the file COPYING. */

/* primitive to delete entries from the nickname history */

#include <config.h>
#include "icb.h"
#include "externs.h"

c_hdel(TARGLIST)
TARGDEFS
{
	if (argc == 1 || !argv[1])
	{
		TRETURNERR("c_hdel: you must specify a nickname with this command.");
	}
	else
	{
		histdel(argv[1]);
		return(TCL_OK);
	}
}
