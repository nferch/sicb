/* Copyright 1990 by Carrick Sean Casey. */
/* For copying and distribution information, see the file COPYING. */

/* primitive to manipulate usage lists */

#include <config.h>
#include <string.h>
#include "icb.h"
#include "externs.h"

c_usage(TARGLIST)
TARGDEFS
{
	static char *usage =
		"usage: c_usage add|delete|list [name] [type args usage]";

	if (argc == 1 || !*argv[1])
		TRETURNERR(usage);

	/* add a usage */
	if (strcmp(argv[1], "add") == 0) {
		if (argc != 6)
			TRETURNERR("usage: c_usage add name type args usage...")
		if (addusage(argv[2], *argv[3], argv[4], argv[5]) < 0)
			TRETURNERR("c_usage: add failed")
		return(TCL_OK);
	}

	/* delete a usage */
	else if (strcmp(argv[1], "delete") == 0) {
		if (argc != 3)
			TRETURNERR("usage: c_usage delete name")
		if (deleteusage(argv[1]) < 0)
			TRETURNERR("c_usage: delete failed")
		return(TCL_OK);
	}

	/* list a usage */
	else if (strcmp(argv[1], "list") == 0) {
		if (listusage(argv[2]) < 0)
			TRETURNERR("c_usage: list failed")
		return(TCL_OK);
	}

	/* unknown function */
	TRETURNERR(usage)
}
