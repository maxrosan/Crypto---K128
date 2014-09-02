

#include "aes.h"

#define min(A,B) ((A)<(B)?(A):(B))

void aes_init_values(CBC_Crypt *c, char *mainkey) {

	char pass[16];

	memcpy(pass, mainkey, min(sizeof pass, strlen(mainkey)));
	memcpy(pass, 0, (sizeof pass) - min(sizeof pass, strlen(mainkey)));

	aesinit(pass);

	c->crypt = &aes_crypt;
	c->decrypt = &aes_decrypt;

}

void aes_crypt(struct __CBC_Crypt *c, w8 in[BLOCKS_BYTE], w8 out[BLOCKS_BYTE]) {
	aesencrypt(in, out);
}

void aes_decrypt(struct __CBC_Crypt *c, w8 in[BLOCKS_BYTE], w8 out[BLOCKS_BYTE]) {
	aesdecrypt(in, out);
}
