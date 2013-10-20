/* Public Domain by Sean Casey 1989. Copyright disclaimed. */

/*

Ask is like "echo" except it never appends a newline.  Some systems use
"echo -n string" and others use "echo string\c" to achieve this effect.
Finding out which one works is a pain so I wrote this. Note that
echo "$*" | tr -d '\012' will work but who wants to sh that every
time they want to ask a question?

*/

#include <stdio.h>

main(argc, argv)
int argc;
char *argv[];
{
	int x;

	for (x = 1; x < argc; x++) {
		fputs(argv[x], stdout);
		if (x < (argc - 1))
			putchar(' ');
	}
	fflush(stdout);
	exit(0);
}
