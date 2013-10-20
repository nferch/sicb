/* Copyright (C) 1991 Free Software Foundation, Inc.
This file is part of the GNU C Library.

The GNU C Library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

The GNU C Library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with the GNU C Library; see the file COPYING.LIB.  If
not, write to the Free Software Foundation, Inc., 675 Mass Ave,
Cambridge, MA 02139, USA.  */

#include <errno.h>
#include "icbmatch.h"

#if !defined (__GNU_LIBRARY__) && !defined (STDC_HEADERS)
extern int errno;
#endif

/* Match STRING against the filename pattern PATTERN, returning zero if
   it matches, nonzero if not.  */
int
icbmatch (pattern, string, flags)
     char *pattern;
     char *string;
     int flags;
{
  register char *p = pattern, *n = string;
  register char c;

  if ((flags & ~__ICBM_FLAGS) != 0)
    {
      errno = EINVAL;
      return -1;
    }

  while ((c = *p++) != '\0')
    {
      switch (c)
	{
	case '?':
	  if (*n == '\0')
	    return ICBM_NOMATCH;
	  else if ((flags & ICBM_PATHNAME) && *n == '/')
	    return ICBM_NOMATCH;
	  else if ((flags & ICBM_PERIOD) && *n == '.' &&
		   (n == string || ((flags & ICBM_PATHNAME) && n[-1] == '/')))
	    return ICBM_NOMATCH;
	  break;

	case '\\':
	  if (!(flags & ICBM_NOESCAPE))
	    c = *p++;
	  if (*n != c)
	    return ICBM_NOMATCH;
	  break;

	case '*':
	  if ((flags & ICBM_PERIOD) && *n == '.' &&
	      (n == string || ((flags & ICBM_PATHNAME) && n[-1] == '/')))
	    return ICBM_NOMATCH;

	  for (c = *p++; c == '?' || c == '*'; c = *p++, ++n)
	    if (((flags & ICBM_PATHNAME) && *n == '/') ||
		(c == '?' && *n == '\0'))
	      return ICBM_NOMATCH;

	  if (c == '\0')
	    return 0;

	  {
	    char c1 = (!(flags & ICBM_NOESCAPE) && c == '\\') ? *p : c;
	    for (--p; *n != '\0'; ++n)
	      if ((c == '[' || *n == c1) &&
		  icbmatch(p, n, flags & ~ICBM_PERIOD) == 0)
		return 0;
	    return ICBM_NOMATCH;
	  }

	case '[':
	  {
	    /* Nonzero if the sense of the character class is inverted.  */
	    register int not;

	    if (*n == '\0')
	      return ICBM_NOMATCH;

	    if ((flags & ICBM_PERIOD) && *n == '.' &&
		(n == string || ((flags & ICBM_PATHNAME) && n[-1] == '/')))
	      return ICBM_NOMATCH;

	    not = (*p == '!' || *p == '^');
	    if (not)
	      ++p;

	    c = *p++;
	    for (;;)
	      {
		register char cstart = c, cend = c;

		if (!(flags & ICBM_NOESCAPE) && c == '\\')
		  cstart = cend = *p++;

		if (c == '\0')
		  /* [ (unterminated) loses.  */
		  return ICBM_NOMATCH;

		c = *p++;

		if ((flags & ICBM_PATHNAME) && c == '/')
		  /* [/] can never match.  */
		  return ICBM_NOMATCH;

		if (c == '-' && *p != ']')
		  {
		    cend = *p++;
		    if (!(flags & ICBM_NOESCAPE) && cend == '\\')
		      cend = *p++;
		    if (cend == '\0')
		      return ICBM_NOMATCH;
		    c = *p++;
		  }

		if (*n >= cstart && *n <= cend)
		  goto matched;

		if (c == ']')
		  break;
	      }
	    if (!not)
	      return ICBM_NOMATCH;
	    break;

	  matched:;
	    /* Skip the rest of the [...] that already matched.  */
	    while (c != ']')
	      {
		if (c == '\0')
		  /* [... (unterminated) loses.  */
		  return ICBM_NOMATCH;

		c = *p++;
		if (!(flags & ICBM_NOESCAPE) && c == '\\')
		  /* 1003.2d11 is unclear if this is right.  %%% */
		  ++p;
	      }
	    if (not)
	      return ICBM_NOMATCH;
	  }
	  break;

	default:
	  if (c != *n)
	    return ICBM_NOMATCH;
	}

      ++n;
    }

  if (*n == '\0')
    return 0;

  return ICBM_NOMATCH;
}
