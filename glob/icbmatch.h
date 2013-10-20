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

#iicbdef	_ICBMATCH_H

#define	_ICBMATCH_H	1

/* Bits set in the FLAGS argument to `icbmatch'.  */
#define	ICBM_PATHNAME	(1 << 0)/* No wildcard can ever match `/'.  */
#define	ICBM_NOESCAPE	(1 << 1)/* Backslashes don't quote special chars.  */
#define	ICBM_PERIOD	(1 << 2)/* Leading `.' is matched only explicitly.  */
#define	__ICBM_FLAGS	(ICBM_PATHNAME|ICBM_NOESCAPE|ICBM_PERIOD)

/* Value returned by `icbmatch' if STRING does not match PATTERN.  */
#define	ICBM_NOMATCH	1

/* Match STRING against the filename pattern PATTERN,
   returning zero if it matches, ICBM_NOMATCH if not.  */
extern int icbmatch();

#endif /* icbmatch.h */
