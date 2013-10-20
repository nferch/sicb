# Generated automatically from Makefile.in by configure.
#	Copyright (c) 1990 by Carrick Sean Casey.
#	For copying and distribution information, see the file COPYING.

#	Makefile for libtcl.a
#
#	You should not need to modify this makefile.

SHELL=	/bin/sh

subdirs=	readline murgil tcl icb
top_srcdir=	..
srcdir=		.
exec_prefix = ${prefix}

bindir = $(exec_prefix)/bin
libdir = $(prefix)/lib
mandir = $(prefix)/man/man1

CC = gcc
CFLAGS = -I. -g -O2 -I$(top_srcdir) -I. -DTCL_VERSION=\"3.3\"
LDFLAGS = 
LIBS = -lnsl -ltermcap 
INSTALL = /usr/bin/install -c
RANLIB = ranlib



SRC = \
	panic.c \
	strchr.c \
	strerror.c \
	strpbrk.c \
	strspn.c \
	strstr.c \
	strtol.c \
	strtoul.c \
	tclAssem.c \
	tclBasic.c \
	tclCmdAH.c \
	tclCmdIZ.c \
	tclExpr.c  \
	tclGlob.c \
	tclHistory.c \
	tclProc.c \
	tclUtil.c

OBJ = \
	panic.o \
	strchr.o \
	strerror.o \
	strpbrk.o \
	strspn.o \
	strstr.o \
	strtol.o \
	strtoul.o \
	tclAssem.o \
	tclBasic.o \
	tclCmdAH.o \
	tclCmdIZ.o \
	tclExpr.o  \
	tclGlob.o \
	tclHistory.o \
	tclProc.o \
	tclUtil.o

all: libtcl.a

libtcl.a: $(PMAKE) $(OBJ)
	rm -f libtcl.a
	ar q libtcl.a $(OBJ)
	-$(RANLIB) libtcl.a

install: libtcl.a


cxref:
	cxref -c $(CFLAGS) $(SRC)

distclean clean:
	rm -f *.o libtcl.a Make.bak core tags test


depend:
	$(MAKEDEPEND) $(DEPENDFLAGS) *.c

# DO NOT DELETE THIS LINE -- make depend depends on it.

strchr.o: string.h
strerror.o: string.h
strpbrk.o: string.h
strspn.o: string.h
strtoul.o: sprite.h  
tclAssem.o: tclInt.h tcl.h 
tclAssem.o: string.h
tclBasic.o:  string.h
tclBasic.o: tclInt.h tcl.h
tclCmdAH.o:  string.h 
tclCmdAH.o: tclInt.h tcl.h
tclCmdIZ.o:  string.h
tclCmdIZ.o: tclInt.h tcl.h
tclExpr.o: tcl.h tclInt.h
tclGlob.o:  string.h 
tclGlob.o: tcl.h
tclHistory.o: tclInt.h tcl.h 
tclHistory.o:  string.h
tclProc.o:  string.h 
tclProc.o: tclInt.h tcl.h
tclUtil.o:  string.h tcl.h
tclUtil.o: tclInt.h
