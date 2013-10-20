#ifndef __ICBCRYPT_H
#define __ICBCRYPT_H 1

char * blowfishEncrypt(char * nick, unsigned char * message, int len);
unsigned char * blowfishDecrypt(char * nick, char * message, int *outlen);
char *radix64encode(unsigned char * data, int len);
unsigned char * radix64decode(char * data, int len, int *outlen);
char * dhGetPValue(char * nick, int valueOnDiskOK);
char * dhMakeKeyPair(char * nick);
int dhSetPValue(char * nick, char * hexPValue);
void makeContextForNick(char * nick);

#endif
