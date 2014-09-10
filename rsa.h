

#ifndef RSA_H
#define RSA_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gmp.h>
#include <time.h>
#include <stdint.h>
#include <memory.h>

#include "cbc.h"

typedef struct __RSA_Crypt {
	CBC_Crypt cbc;
	mpz_t e, d, n;
} RSA_Crypt;

#define RSA_WORD_IN_LEN 8

void rsa_init_values(RSA_Crypt *c, char *e_str, char *d_str, char *n_str);
void rsa_crypt(RSA_Crypt *c, w8 in[BLOCKS_BYTE], w8 out[BLOCKS_BYTE]);
void rsa_decrypt(RSA_Crypt *c, w8 in[BLOCKS_BYTE], w8 out[BLOCKS_BYTE]);
void rsa_power(char *base, char *power, char *mod);

#endif
