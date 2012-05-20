
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <assert.h>

#include "k128.h"

enum { ENCODE, DECODE, MODE_2, MODE_3 };

//! Apaga arquivo
inline static void 
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
	
	char *i_value = 0, *o_value = 0, *p_value = 0;
	int comm;
	int c;
	int whitespace_file = 0;

	while ((c = getopt(argc, argv, "acdi:o:p:12t")) != -1) {
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
			case 'a':
				whitespace_file = 1;
				break;
			default:
				fprintf(stderr, "Argumento desconhecido!\n");
		}
	}

	//tests_functions();
	
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
		CBC_Crypt c;

		assert(i_value != 0);
		assert(o_value != 0);
		assert(p_value != 0);

		k128_init(&c, p_value);
		cbc_encode(&c, i_value, o_value);

		if (whitespace_file) {
			printf("Arquivo %s excluído\n", i_value);
			_removeFile(i_value);
		}

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
