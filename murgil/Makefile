# Generated automatically from Makefile.in by configure.
a=	/bin/sh

subdirs=	readline murgil tcl icb
top_srcdir=	..
srcdir=		.
exec_prefix = ${prefix}

bindir = $(exec_prefix)/bin
libdir = $(prefix)/lib
mandir = $(prefix)/man/man1

CC = gcc
CFLAGS = -g -O2 -I..
LDFLAGS = 
LIBS = -lnsl -ltermcap 
INSTALL = /usr/bin/install -c
RANLIB = ranlib

SRC = \
	clientserve.c \
	contoport.c \
	discuser.c \
	getaddr.c \
	getrname.c \
	globals.c \
	ignore.c \
	makeport.c \
	newconnect.c \
	packetsend.c \
	readpacket.c \
	sendpacket.c \
	serverserve.c \
	setdelay.c 

OBJ = \
	clientserve.o \
	contoport.o \
	discuser.o \
	getaddr.o \
	getrname.o \
	globals.o \
	ignore.o \
	makeport.o \
	newconnect.o \
	packetsend.o \
	readpacket.o \
	sendpacket.o \
	serverserve.o \
	setdelay.o 

all: libmurgil.a

libmurgil.a: $(PMAKE) $(OBJ)
	rm -f libmurgil.a
	ar q libmurgil.a $(OBJ)
	-$(RANLIB) libmurgil.a

install: libmurgil.a

lint:
	lint $(LFLAGS) $(SRC)

cxref:
	cxref -c $(CFLAGS) $(SRC)

dstrip:
	cp Make Make.bak
	perl ../tools/dstrip.pl < Make > Make.o
	mv Make.o Make

distclean clean:
	rm -f *.o libmurgil.a  core tags

saber_src:
	#load *.c

depend:
	$(MAKEDEPEND) $(DEPENDFLAGS) *.c

# DO NOT DELETE THIS LINE -- make depend depends on it.

