#include <openssl/evp.h>
#include <openssl/bn.h>
#include <openssl/dh.h>
#include <stdlib.h>
#include "icbcrypt.h"

typedef struct {
	char nick[32];
	DH *dh;
	EVP_CIPHER_CTX * encContext;
	EVP_CIPHER_CTX * decContext;
} nickcontext;
	
nickcontext * nickcontexts[512];
int maxnickcontext = -1;
DH *defDH = NULL;

nickcontext * getNickContext(char * nick, int create) {
	int i;
	for (i = 0; i<=maxnickcontext; i++) {
		if (!strcasecmp(nickcontexts[i]->nick,nick)) {
			return nickcontexts[i];
		}
	}
	if (create) {
		makeContextForNick(nick);
		return getNickContext(nick,0);
	} else {
		return NULL;
	}
}

EVP_CIPHER_CTX * getContextForNick(char * nick,int enc) {
	nickcontext * thecontext = getNickContext(nick,0);
	if (thecontext == NULL) {
		return NULL;
	}
	if (enc) {
		return thecontext->encContext;
	} else {
		return thecontext->decContext;
	}
}

void makeContextForNick(char * nick) {
	int conpos = 0;

	for (conpos = 0; conpos <= maxnickcontext; conpos++) {
		if (!strcmp(nickcontexts[conpos]->nick,nick)) {
			nickcontexts[conpos] = malloc(sizeof(nickcontext));
			strcpy(nickcontexts[maxnickcontext]->nick,nick);
			break;
		}
	}

	if (conpos > maxnickcontext) {
		nickcontexts[++maxnickcontext] = malloc(sizeof(nickcontext));
		strcpy(nickcontexts[maxnickcontext]->nick,nick);
	}

	nickcontexts[maxnickcontext]->encContext = NULL;
	nickcontexts[maxnickcontext]->decContext = NULL;
}
	

char * blowfishEncrypt(char * nick, unsigned char * message, int len) {
	char ivec[8];
	unsigned char out[1024];
	int ol;
	int outlen;
	char * data64;
	EVP_CIPHER_CTX * ctx;

	ctx = getContextForNick(nick,1);
	if (ctx == NULL) {
		return NULL;
	}
	EVP_EncryptUpdate(ctx,out,&ol,message,len);
	outlen = ol;
	data64 = radix64encode(out,outlen);
	return data64;
}

unsigned char * blowfishDecrypt(char * nick, char * message, int *outlen) {
	char ivec[8];
	unsigned char * out = malloc(1024);
	int ol;
	int datalen;
	unsigned char * data;
	EVP_CIPHER_CTX * ctx;

	ctx = getContextForNick(nick,0);
	if (ctx == NULL) {
		return NULL;
	}
	data = radix64decode(message,strlen(message),&datalen);
	EVP_DecryptUpdate(ctx,out,&ol,data,datalen);
	*outlen = ol;
	EVP_DecryptFinal(ctx,out+ol,&ol);
	*outlen += ol;
	free(data);
	return out;
}

char radix64enctable[64] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
	'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3',
	'4', '5', '6', '7', '8', '9', '+', '/'};

unsigned char radix64dectable[123] = {
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128,  62, 128, 128, 128,  63, 
	 52,  53,  54,  55,  56,  57,  58,  59, 
	 60,  61, 128, 128, 128,   0, 128, 128, 
	128,   0,   1,   2,   3,   4,   5,   6, 
	  7,   8,   9,  10,  11,  12,  13,  14, 
	 15,  16,  17,  18,  19,  20,  21,  22, 
	 23,  24,  25, 128, 128, 128, 128, 128, 
	128,  26,  27,  28,  29,  30,  31,  32, 
	 33,  34,  35,  36,  37,  38,  39,  40, 
	 41,  42,  43,  44,  45,  46,  47,  48, 
	 49,  50,  51};

char *radix64encode(unsigned char * data, int len) {
	char * encdata = malloc(len * 4 / 3 + 4);
	int i = 0;
	int o = 0;
	int n; 
	while (i < len) {
		unsigned char a,b,c = 0;
		n = 0;
		a = (unsigned char)data[i++];
		n++;
		if (i < len) {
			b = (unsigned char)data[i++];
			n++;
		} else {
			b = 0;
		}
		if (i < len) {
			c = (unsigned char)data[i++];
			n++;
		} else {
			c = 0;
		}
		encdata[o++] = radix64enctable[a >> 2];
		encdata[o++] = radix64enctable[((a & 3) << 4) | (b >> 4)];
		encdata[o++] = radix64enctable[((b & 0xF) << 2) | (c >> 6)];
		encdata[o++] = radix64enctable[c & 0x3F];
		if (n < 3) {
			encdata[o-1] = '=';
			if (n < 2) {
				encdata[o-2] = '=';
			}
		}
		encdata[o] = '\0';
	}
	return encdata;
}

unsigned char * radix64decode(char * data, int len, int *outlen) {
	unsigned char * decdata = malloc(len * 3 / 4 + 3);
	int i = 0;
	int o = 0;

	while (i < len) {
		int n = 0;
		unsigned char a, b, c, d = 0;
		a = radix64dectable[data[i++]];
		b = radix64dectable[data[i++]];
		c = radix64dectable[data[i++]];
		d = radix64dectable[data[i++]];
		decdata[o++] = (a << 2) | (b >> 4);
		decdata[o++] = (b << 4) | (c >> 2);
		decdata[o++] = (c << 6) | (d);
		n += 3;
	}
	if (data[i-2] == '=') {
		o -= 2;
	} else if (data[i-1] == '=') {
		o -= 1;
	}
	*outlen = o;
	return decdata;
}

void dhGenCallback(int a, int b, void * ugh) {
        if (a == 3 && b == 0) {
                *(int *)ugh = 0;
		printf("\n");
	}
	printf(".");
	fflush(0);
}

char * dhGetPValue(char * nick) {
	nickcontext * thecontext = getNickContext(nick,1);
	int WAITING = 1;

	if (thecontext == NULL)
		return NULL;
	if (defDH == NULL) {
		printf("Making DH parameters.\n");
		defDH = DH_generate_parameters(256,5,(void *)dhGenCallback,(void *)&WAITING);
		while (WAITING) {
			sleep(1);
		}
	}
	thecontext->dh = malloc(sizeof(DH));
	memcpy(thecontext->dh,defDH,sizeof(DH));
	
	return BN_bn2hex(thecontext->dh->p);
}


int dhSetPValue(char * nick, char * hexPValue) {
	nickcontext * thecontext = getNickContext(nick,1);
	DH * tempDH = DH_new();
	int checkcodes = 0;
	int rc;

	rc = BN_dec2bn(&(tempDH->g),"5");
	rc = BN_hex2bn(&(tempDH->p),hexPValue);


	if (!DH_check(tempDH,&checkcodes)) {
		DH_free(tempDH);
		return 1;
	} else if (checkcodes > 0) {
		DH_free(tempDH);
		return 1;
	} else {
		thecontext->dh = tempDH;
		return 0;
	}
}

char * dhMakeKeyPair(char * nick) {
	nickcontext * thecontext = getNickContext(nick,0);

	

	DH_generate_key(thecontext->dh);
	
	return BN_bn2hex(thecontext->dh->pub_key);
}

void makeEncContexts(char * nick, unsigned char * secret) {
	nickcontext * thecontext = getNickContext(nick,0);
	char * ivec = malloc(sizeof(char) * 8);

	memset(ivec,0,8);
	thecontext->encContext = malloc(sizeof(EVP_CIPHER_CTX));
	thecontext->decContext = malloc(sizeof(EVP_CIPHER_CTX));
	EVP_EncryptInit(thecontext->encContext,EVP_bf_cfb(),secret,ivec);
	EVP_DecryptInit(thecontext->decContext,EVP_bf_cfb(),secret,ivec);
}

int dhGenerateBFKey(char * nick, char * hexPubKey) {
	int secretsize;
	unsigned char secret[2048];
	BIGNUM * peerpubkey = NULL;

	nickcontext * thecontext = getNickContext(nick,0);

	if (thecontext == NULL) {
		return 1;
	}
	
	peerpubkey = BN_new();
	if (!BN_hex2bn(&peerpubkey,hexPubKey)) {
		return 1;
	}

	if (thecontext->dh == NULL) {
		return 1;
	}
	
	if ((secretsize = DH_compute_key(secret,peerpubkey,thecontext->dh)) < 0) {
		return 1;
	}

	secret[secretsize] = '\0';
	makeEncContexts(nick,secret);
	return 0;
}

