#include <config.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef HAVE_READLINE_H
#include <readline.h>
#endif

#ifdef HAVE_READLINE_READLINE_H
#include <readline/readline.h>
#endif

#include "icb.h"
#include "externs.h"

char* basename(char*);
/*
 * readlineinit - set up for readline()
 */

readlineinit()
{
char *editor;
int handletab();

/*
 * bind the tab key
 */
rl_bind_key_in_map('\t', handletab, vi_insertion_keymap);
rl_bind_key_in_map('\t', handletab, vi_movement_keymap);
rl_bind_key_in_map('\t', handletab, emacs_standard_keymap);

rl_vi_editing_mode(1,0); /* default to vi, dammit. */
gv.editmode = "vi";

editor = getenv("EDITOR");
if (!editor)
	editor = (char *)getenv("VISUAL");

if (editor != NULL)
   if (strcmp((char *)basename(editor),"emacs")==0)
   {
	rl_emacs_editing_mode(1,0);
	gv.editmode = "emacs";
   }

}
