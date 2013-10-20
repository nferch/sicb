#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "icbcrypt.h"
#include <string.h>

static int
not_here(char *s)
{
    croak("%s not implemented on this architecture", s);
    return -1;
}

static double
constant(char *name, int arg)
{
    errno = 0;
    switch (*name) {
    }
    errno = EINVAL;
    return 0;

not_there:
    errno = ENOENT;
    return 0;
}


MODULE = IcbCrypt		PACKAGE = IcbCrypt		


double
constant(name,arg)
	char *		name
	int		arg


int
dhSetPValue(nick,hexPValue)
	char * nick
	char * hexPValue


char *
dhGetPValue(nick,valueOnDiskOK)
	char * nick
	int valueOnDiskOK


char *
dhMakeKeyPair(nick)
	char * nick

int
dhGenerateBFKey(nick,hexPubKey)
	char * nick
	char * hexPubKey

char *
blowfishEncrypt(nick, message)
	char * nick
	unsigned char * message
    CODE:
	{
		int len = strlen(message);
		RETVAL = blowfishEncrypt(nick,message,len);
	}
  OUTPUT:
		RETVAL

char *
blowfishDecrypt(nick,message)
	char * nick
	unsigned char * message
    CODE:
	{
		int len;
		unsigned char * output = blowfishDecrypt(nick,message,&len);
		char * safeOutput = malloc(len + 1);
		strncpy(safeOutput,output,len);
		*(safeOutput + len) = '\0';
		free(output);
		RETVAL = safeOutput;
	}
  OUTPUT:
		RETVAL
