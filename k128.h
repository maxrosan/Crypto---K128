
#ifndef K128H
#define K128H

#include "cbc.h"

struct __K128_Crypt {
	CBC_Crypt cbc;
	w64  *__skeys;
};

void generate_keys(char *key, w64** k);
void tests_functions(void);
void k128_init(CBC_Crypt *c, char *mainkey);
void randomness_k128_mode(char *filename, char *password, int mode);
#endif
