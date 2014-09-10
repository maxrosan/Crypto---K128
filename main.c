
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "aes.h"
#include "k128.h"
#include "rsa.h"

enum { ENCODE, DECODE, MODE_2, MODE_3, GENERATE_RSA_KEYS, GENERATE_SYMMETRIC_KEY, EXPONENTIATION };

/*! Apaga arquivo */
static void 
_removeFile(char *filename) {
	FILE *fp;

	assert(filename != 0);
	
	fp = fopen(filename, "rb+");
	if (fp != 0) {
		long size;
		fseek(fp, 0L, SEEK_END);
		size = ftell(fp);
		fseek(fp, 0L, SEEK_SET);

		while (size--) {
			fwrite("\0", 1, 1, fp);
		}

		fclose(fp);

		unlink(filename);
	} else {
		fprintf(stderr, "Arquivo a ser removido não existe\n");
	}
}

int main(int argc, char **argv) {
	
	char *i_value = 0, *o_value = 0, *p_value = 0, *algorithm_name_value, *g_value, *expr;
	int comm;
	int c;
	int whitespace_file = 0;

	while ((c = getopt(argc, argv, "acdi:o:p:12tf:g:e:")) != -1) {
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
#if ENABLE_TESTS
			case 't':
				tests_functions();
				break;
#endif
			case 'a':
				whitespace_file = 1;
				break;
			case 'f':
				algorithm_name_value = (char*) malloc(strlen(optarg) + 1);
				strcpy(algorithm_name_value, optarg);
				break;
			case 'g':
				g_value = (char*) malloc(strlen(optarg) + 1);
				strcpy(g_value, optarg);

				if (strcmp("sym", g_value) == 0) {
					comm = GENERATE_SYMMETRIC_KEY;	
				} else if (strcmp("rsa", g_value) == 0) {
					comm = GENERATE_RSA_KEYS;
				}
		
				break;
			case 'e':

				comm = EXPONENTIATION;

				expr = (char*) malloc(strlen(optarg) + 1);
				strcpy(expr, optarg);

				break;

			default:
				fprintf(stderr, "Argumento desconhecido!\n");
		}
	}

	/* tests_functions(); */

	if ((comm == ENCODE || comm == DECODE) && 
	 (algorithm_name_value == NULL || 
		strcmp(algorithm_name_value, "rsa") &&
		strcmp(algorithm_name_value, "aes") &&
		strcmp(algorithm_name_value, "k128"))) {

		fprintf(stderr, "Um algoritmo deve ser escolhido: K128, AES-NI\n");
		return EXIT_FAILURE;

	}
	
	if (p_value != 0) {
		if (strlen(p_value) < 8) {
			fprintf(stderr, "A senha deve conter pelo menos 8 caracteres\n");
			return EXIT_FAILURE;
		}

		{
			int num_letras = 0, num_algarismo = 0;
			char *pt = p_value;

			while (*pt && ((num_letras < 2) || (num_algarismo < 2))) {
				if (*pt >= 'a' && *pt <= 'z') num_letras++;
				else if (*pt >= 'A' && *pt <= 'Z') num_letras++;
				else if (*pt >= '0' && *pt <= '9') num_algarismo++;
				pt++;
			}

			if (num_letras < 2 || num_algarismo < 2) {
				fprintf(stderr, "%d letras e %d algarismos decimais. Número inválido de letras ou algarismos decimais\n",
				 num_letras, num_algarismo);
				return EXIT_FAILURE;
			}
		}
	}
	
	if (comm == ENCODE) {

		CBC_Crypt *c;

		assert(i_value != 0);
		assert(o_value != 0);
		assert(p_value != 0);

		if (strcmp("k128", algorithm_name_value) == 0) {
			c = (CBC_Crypt*) malloc(sizeof(K128_Crypt));
			cbc_init(c);
			k128_init(c, p_value);
		} else if (strcmp("aes", algorithm_name_value) == 0) {
			c = (CBC_Crypt*) malloc(sizeof(CBC_Crypt));
			cbc_init(c);
			aes_init_values(c, p_value);
		} else if (strcmp("rsa", algorithm_name_value) == 0) {
			char *e_key, *n_key;
			
			e_key = strtok(p_value, "@");
			n_key = strtok(NULL, "@");

			c = (CBC_Crypt*) malloc(sizeof(RSA_Crypt));
			ebc_init(c);
			rsa_init_values((RSA_Crypt*) c, e_key, NULL, n_key);
		}

		c->encode(c, i_value, o_value);

		if (whitespace_file) {
			printf("Arquivo %s excluído\n", i_value);
			_removeFile(i_value);
		}

		free(c);

	} else if (comm == DECODE) {
		CBC_Crypt *c;

		assert(i_value != 0);
		assert(o_value != 0);
		assert(p_value != 0);

		if (strcmp("k128", algorithm_name_value) == 0) {
			c = (CBC_Crypt*) malloc(sizeof(K128_Crypt));
			cbc_init(c);
			k128_init(c, p_value);
		} else if (strcmp("aes", algorithm_name_value) == 0) {
			c = (CBC_Crypt*) malloc(sizeof(CBC_Crypt));
			cbc_init(c);
			aes_init_values(c, p_value);
		} else if (strcmp("rsa", algorithm_name_value) == 0) {
			char *d_key, *n_key;
			
			d_key = strtok(p_value, "@");
			n_key = strtok(NULL, "@");

			c = (CBC_Crypt*) malloc(sizeof(RSA_Crypt));
			ebc_init(c);
			rsa_init_values((RSA_Crypt*) c, NULL, d_key, n_key);
		}

		c->decode(c, i_value, o_value);

		free(c);

	} else if (comm == MODE_2) {
		assert(p_value != 0);
		assert(i_value != 0);

		randomness_k128_mode(i_value, p_value, 1);
	} else if (comm == MODE_3) {
		assert(p_value != 0);
		assert(i_value != 0);

		randomness_k128_mode(i_value, p_value, 2);

	} else if (comm == GENERATE_SYMMETRIC_KEY) {

		char key[17];
		int i;

		memset(key, 0, sizeof key);
		
		srand(time(NULL));

		for (i = 0; i < ((sizeof key) - 1); i++) {
			key[i] = (rand() & 1) ? ('0' + (rand() % 10)) :  ('A' + (rand() % 4) ) ;
		}

		fprintf(stderr, "password = %s\n", key);

	} else if (comm == GENERATE_RSA_KEYS) {
		char *e_str, *d_str, *n_str;

		rsa_generate_keys(&e_str, &d_str, &n_str);
		
		printf("(public) e = %s@%s\n", e_str, n_str);
		printf("(private) d = %s@%s\n", d_str, n_str);

		free(e_str);
		free(d_str);
		free(n_str);
	} else if (comm == EXPONENTIATION) {


		char *base, *power, *mod;

		base = strtok(expr, "@");
		power = strtok(NULL, "@");
		mod = strtok(NULL, "@");

		rsa_power(base, power, mod);
	}


	return EXIT_SUCCESS;
}
