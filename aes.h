#ifndef AESH
#define AESH

#include <stdint.h>
#include <math.h>
#include "cbc.h"

void aesinit(char *password);
void aesencrypt(char *msg, char *crypted);
void aesdecrypt(char *crypted, char *msg);

void aes_init_values(CBC_Crypt *c, char *mainkey);
void aes_crypt(struct __CBC_Crypt *c, w8 in[BLOCKS_BYTE], w8 out[BLOCKS_BYTE]);
void aes_decrypt(struct __CBC_Crypt *c, w8 in[BLOCKS_BYTE], w8 out[BLOCKS_BYTE]);

#endif
