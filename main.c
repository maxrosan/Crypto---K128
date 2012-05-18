
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <assert.h>

#include "k128.h"

enum { ENCODE, DECODE, MODE_2, MODE_3 };

int main(int argc, char **argv) {
	
	char *i_value = 0, *o_value = 0, *p_value = 0;
	int comm;
	int c;

	while ((c = getopt(argc, argv, "cdi:o:p:12t")) != -1) {
		switch (c) {
			case 'c':
				comm = ENCODE;
				break;
			case 'd':
				comm = DECODE;
				break;
			case '1':
				comm = MODE_2;
				break;
			case '2':
				comm = MODE_3;
				break;
			case 'i':
				i_value = (char*) malloc(strlen(optarg) + 1);
				strcpy(i_value, optarg);
				break;
			case 'o':
				o_value = (char*) malloc(strlen(optarg) + 1);
				strcpy(o_value, optarg);
				break;
			case 'p':
				p_value = (char*) malloc(strlen(optarg) + 1);
				strcpy(p_value, optarg);
				break;
			case 't':
				tests_functions();
				break;
			default:
				fprintf(stderr, "Argumento desconhecido!\n");
		}
	}

	//tests_functions();
	
	if (comm == ENCODE) {
		CBC_Crypt c;

		assert(i_value != 0);
		assert(o_value != 0);
		assert(p_value != 0);

		k128_init(&c, p_value);
		cbc_encode(&c, i_value, o_value);
	} else if (comm == DECODE) {
		CBC_Crypt c;

		assert(i_value != 0);
		assert(o_value != 0);
		assert(p_value != 0);

		k128_init(&c, p_value);
		cbc_decode(&c, i_value, o_value);
	} else if (comm == MODE_2) {
		assert(p_value != 0);
		assert(i_value != 0);

		randomness_k128_mode(i_value, p_value, 1);
	} else if (comm == MODE_3) {
		assert(p_value != 0);
		assert(i_value != 0);

		randomness_k128_mode(i_value, p_value, 2);
	}
	return EXIT_SUCCESS;
}
