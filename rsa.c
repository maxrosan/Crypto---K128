
#include "rsa.h"

void rsa_generate_keys(char** e_str, char** d_str, char **n_str) {

	int i;
	size_t size;
	gmp_randstate_t randState;
	mp_bitcnt_t nbit = RSA_WORD_IN_LEN * 8 * 2;
	int bit;
	mpz_t p, q, n, phi_n, a, b, e, d, one, m;
	char *output = NULL;

	srand(time(NULL));

	mpz_init2(p, nbit / 2);
	mpz_init2(q, nbit / 2);
	mpz_init(n);
	mpz_init(phi_n);

	mpz_init(a);
	mpz_init(b);
	mpz_init(e);
	mpz_init(d);
	mpz_init(one);
	mpz_init(m);

	mpz_set_ui(one, 1);

	gmp_randinit_default(randState);

calc_again:
	for (i = 0; i < (nbit / 2); i++) {
		bit = rand() & 1;
		mpz_setbit(p, i);
		if (bit) {
			mpz_setbit(p, i);
		} else {
			mpz_clrbit(p, i);
		}

		bit = rand() & 1;
		mpz_setbit(q, i);
		if (bit) {
			mpz_setbit(q, i);
		} else {
			mpz_clrbit(q, i);
		}
	}

	mpz_nextprime(p, p);
	mpz_nextprime(q, q);

	if (mpz_cmp(p, q) == 0 || mpz_probab_prime_p(p, 1 << 20) == 0 || mpz_probab_prime_p(q, 1 << 20) == 0) {
		goto calc_again;
	}

	mpz_mul(n, p, q);
	
	mpz_sub_ui(a, p, 1);
	mpz_sub_ui(b, q, 1);

	mpz_mul(phi_n, a, b);

	do {
		mpz_urandomm(e, randState, phi_n);
		mpz_gcd(a, e, phi_n);
	} while (mpz_cmp_ui(a, 1) != 0);

	mpz_gcdext (one, d, a, e, phi_n);

	if (mpz_sgn(d) < 0) {
		mpz_set(d, a);
	}

	mpz_mul(b, d, e);
	mpz_mod(b, b, phi_n);

	output = mpz_get_str(NULL, 16, b);
	fprintf(stderr, "d . e (mod phi_n) = %s\n", output);
	free(output);

	if (mpz_cmp_ui(b, 1) != 0) {
		goto calc_again;
	}
	

	*e_str = mpz_get_str(NULL, 16, e);
	*d_str = mpz_get_str(NULL, 16, d);
	*n_str = mpz_get_str(NULL, 16, n);

	mpz_clear(p);
	mpz_clear(q);
	mpz_clear(n);
	mpz_clear(phi_n);
	mpz_clear(a);
	mpz_clear(b);
	mpz_clear(e);
	mpz_clear(d);
	mpz_clear(one);
	mpz_clear(m);
}

void rsa_init_values(RSA_Crypt *c, char *e_str, char *d_str, char *n_str) {
	
	CBC_Crypt *cbc = (CBC_Crypt*) c;

	cbc->crypt = &rsa_crypt;
	cbc->decrypt = &rsa_decrypt;

	mpz_init(c->e);
	mpz_init(c->d);
	mpz_init(c->n);

	if (e_str != NULL) {
		mpz_set_str(c->e, e_str, 16);
	}

	if (d_str != NULL) {
		mpz_set_str(c->d, d_str, 16);
	}

	mpz_set_str(c->n, n_str, 16);

	cbc->word_length = RSA_WORD_IN_LEN;

}

void rsa_crypt(RSA_Crypt *c, w8 in[BLOCKS_BYTE], w8 out[BLOCKS_BYTE]) {

	mpz_t x_in, x_out;
	int i;

	mpz_init(x_in);
	mpz_init(x_out);

	memset(out, 0, BLOCKS_BYTE);

	mpz_import (x_in, BLOCKS_BYTE, 1, 1, -1, 0, in);
	mpz_powm (x_out, x_in, c->e, c->n);
	mpz_export (out, NULL, 1, 1, -1, 0, x_out);

	mpz_clear(x_in);
	mpz_clear(x_out);
}

void rsa_decrypt(RSA_Crypt *c, w8 in[BLOCKS_BYTE], w8 out[BLOCKS_BYTE]) {

	mpz_t x_in, x_out;
	int i;

	mpz_init(x_in);
	mpz_init(x_out);

	memset(out, 0, BLOCKS_BYTE);

	mpz_import (x_in, BLOCKS_BYTE, 1, 1, -1, 0, in);
	mpz_powm (x_out, x_in, c->d, c->n);
	mpz_export (out, NULL, 1, 1, -1, 0, x_out);

	mpz_clear(x_in);
	mpz_clear(x_out);
}

void rsa_free(RSA_Crypt *c) {
	
	mpz_clear(c->e);
	mpz_clear(c->d);
	mpz_clear(c->n);

}

void rsa_power(char *base, char *power, char *mod) {

	mpz_t x_base, x_power, x_mod, x_result;

	mpz_init(x_base);
	mpz_init(x_power);
	mpz_init(x_mod);
	mpz_init(x_result);

	mpz_set_str(x_base, base, 16);
	mpz_set_str(x_power, power, 16);
	mpz_set_str(x_mod, mod, 16);

	mpz_powm (x_result, x_base, x_power, x_mod);
	mpz_add(x_result, x_result, x_mod);
	mpz_mod(x_result, x_result, x_mod);

	fprintf(stderr, "out = ");
	mpz_out_str(stderr, 16, x_result);
	fprintf(stderr, "\n");

	mpz_clear(x_base);
	mpz_clear(x_power);
	mpz_clear(x_mod);
	mpz_clear(x_result);
}
