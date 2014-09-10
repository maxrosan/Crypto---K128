
#include "cbc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void uint64_to_bytes(w64 in, int n, w8* out) {
	int i;

	for (i = 7; i >= 0; i--) {
		out[i + n] = in & 0xFF;
		in = in >> 8;
	}
}

/*!
 * \brief Converte uma sequência de 8 bytes de um array em palavra de 64 bits
 * \param buffer : Array que contém a sequência
 * \param n : Posição do primeiro elemento da sequência a ser convertida
 * \return Palavra resultante da conversão
 */
static 
w64 to_uint64(w8* buffer, int n) {
	return (
	(w64)buffer[n] << 56 | (w64)buffer[n+1] << 48 | 
	(w64)buffer[n+2] << 40  | (w64)buffer[n+3] << 32 | 
	(w64) buffer[n+4] << 24 | (w64)buffer[n+5] << 16 |
	(w64)buffer[n+6] << 8  | (w64)buffer[n+7]);
}


/*!
 * \brief XOR dos arrays 'a' e 'b' e coloca o array resultante em 'result'
 * \param a : Operando do xor
 * \param b : Operando do xor
 * \param result : Resultado do xor
 * \return void
*/
static void
xor_block (w8 a[BLOCKS_BYTE], w8 b[BLOCKS_BYTE], w8 result[BLOCKS_BYTE]) {
	int i;

	for (i = 0; i < BLOCKS_BYTE; i++) {
		result[i] = a[i] ^ b[i];
	}
}

/*!
 * \brief Criptografar arquivo no modo CBC
 * \param c : Configurações da criptografia
 * \param filename : Nome do arquivo que deve ter conteúdo criptografado
 * \param fileoutput : Nome do arquivo que receberá o conteúdo criptografado
 * \return void
*/
void
cbc_encode(CBC_Crypt *cbc, char *filename, char *fileoutput) {
	FILE *in;
	FILE *out;

	LOG("Criptografia");

	in = fopen(filename, "rb+");
	out = fopen(fileoutput, "wb+");

	if (out == 0) {
		fprintf(stderr, "Erro ao abrir %s para escrita\n", fileoutput);
	} else if (in != 0) {
		w8 buf[BLOCKS_BYTE], buf_out[BLOCKS_BYTE], C[BLOCKS_BYTE];
		size_t sz;
		int i, ok;
		w64 save_size;
		int c = 1;

		memset(C, 0xFF, BLOCKS_BYTE);

		fseek(in, 0, SEEK_END);
		save_size = (w64) ftell(in);
		fseek(in, 0, SEEK_SET);

		uint64_to_bytes(save_size, 0, buf);

		LOG("P[%d] = " WFRT " " WFRT, c, WORD(buf), WORD(buf + 8));

		/* O primeiro bloco deve conter o tamanho do arquivo */
		xor_block(buf, C, buf_out);
		cbc->crypt(cbc, buf_out, buf);
		fwrite(buf, BLOCKS_BYTE, 1, out);
		memcpy(C, buf, BLOCKS_BYTE);

		LOG("C[%d] = " WFRT " " WFRT, c, WORD(buf), WORD(buf + 8));

		while (!feof(in)) {
			c++;
			memset(buf, 0xFF, BLOCKS_BYTE);
			sz = 0;
			ok = 1;

			for (i = 0; (i < BLOCKS_BYTE) && ok; i++) {
				ok = fread(buf+i, 1, 1, in);
				sz += ok;
			}

			LOG("P[%d] = " WFRT " " WFRT, c, WORD(buf), WORD(buf + 8));

			xor_block(buf, C, buf_out);
			cbc->crypt(cbc, buf_out, buf); /* C' = crypt(P ^ C) */
			fwrite(buf, BLOCKS_BYTE, 1, out);
			memcpy(C, buf, BLOCKS_BYTE);

			LOG("C[%d] = " WFRT " " WFRT, c, WORD(buf), WORD(buf + 8));

		}

		fclose(in);
		fclose(out);
	} else {
		fprintf(stderr, "Erro ao abrir o arquivo %s\n", filename);
	}
}

/*!
 * \brief Descriptografar arquivo no modo CBC
 * \param c : Configurações da criptografia
 * \param filename : Nome do arquivo que deve ter conteúdo descriptografado
 * \param fileoutput : Nome do arquivo que receberá o conteúdo descriptografado
 * \return void
*/
void
cbc_decode(CBC_Crypt *cbc, char *filename, char *fileoutput) {
	FILE *in;
	FILE *out;

	LOG("Descriptografia");

	in = fopen(filename, "rb+");
	out = fopen(fileoutput, "wb+");

	if (out == 0) {
		fprintf(stderr, "Erro ao abrir %s para escrita\n", fileoutput);
	} else if (in != 0) {
		w8 buf[BLOCKS_BYTE], buf_out[BLOCKS_BYTE], C_new[BLOCKS_BYTE], C[BLOCKS_BYTE];
		size_t sz;
		int i, ok;
		w64 save_size;
		int c = 1;

		memset(C, 0xFF, BLOCKS_BYTE);

		fread(buf, BLOCKS_BYTE, 1, in);

		LOG("C[%d] = " WFRT " " WFRT, c, WORD(buf), WORD(buf + 8));

		/* o primeiro bloco contém o tamanho do arquivo */
		memcpy(C_new, buf, BLOCKS_BYTE);
		cbc->decrypt(cbc, buf, buf_out);
		xor_block(buf_out, C, buf);
		save_size = to_uint64(buf, 0);
		/* xor_block(buf, C, C_new); */
		memcpy(C, C_new, BLOCKS_BYTE);
		
		LOG("P[%d] = " WFRT " " WFRT, c, WORD(buf), WORD(buf + 8));

		i = 0;
		while (fread(buf, BLOCKS_BYTE, 1, in) != 0) { /* = C' */
			c++;
			LOG("C[%d] = " WFRT " " WFRT, c, WORD(buf), WORD(buf + 8));

			memcpy(C_new, buf, BLOCKS_BYTE);
			cbc->decrypt(cbc, buf, buf_out); /* decrypt(C') */
			LOG("D(C[%d]) = " WFRT " " WFRT, c, WORD(buf_out), WORD(buf_out + 8));
			xor_block(buf_out, C, buf); /* decrypt(C') ^ C */
			LOG("C = " WFRT " " WFRT, c, WORD(C), WORD(C + 8));
			LOG("D(C[%d]) ^ C = " WFRT " " WFRT, c, WORD(buf), WORD(buf + 8));
			if ((i + BLOCKS_BYTE) > save_size) {
				fwrite(buf, save_size % BLOCKS_BYTE, 1, out);
			} else {
				i += BLOCKS_BYTE;
				fwrite(buf, BLOCKS_BYTE, 1, out); /* P = decrypt(C') ^ C */
			}
			/* xor_block(buf, C, C_new); */
			memcpy(C, C_new, BLOCKS_BYTE);
			LOG("P[%d] = " WFRT " " WFRT, c, WORD(buf), WORD(buf + 8));

			/*cbc_decrypt(buf, buf_out);
			P = to_uint64(buf_out, 0) ^ C;
			uint64_to_bytes(P, 0, buf_out);
			fwrite(buf_out, 8, 1, out);

			C = P ^ C;*/

		}
		fclose(in);
		fclose(out);
	} else {
		fprintf(stderr, "Erro ao abrir o arquivo %s\n", filename);
	}
}

/*!
 * \brief Criptografar array de bytes no modo CBC
 * \param c : Configurações da criptografia
 * \param num_blocks: número de bytes no VetEntra
 * \param VetEntra : array de bytes que deve ser criptografado
 * \param VetEntraC : array de bytes que conterá o resultado da criptografia
 * \return void
*/
void
cbc_encode_array(CBC_Crypt *c, int num_blocks, w8* VetEntra, w8* VetEntraC) {
		int i;
		w8 *C, *buf;

		C = (w8*) malloc(BLOCKS_BYTE);
		buf = (w8*) malloc(BLOCKS_BYTE);

		assert(C != 0);
		assert(buf != 0);

		memset(C, 0xFF, BLOCKS_BYTE);

		for (i = 0; i < (num_blocks * BLOCKS_BYTE); i += BLOCKS_BYTE) {
			xor_block(VetEntra + i, C, buf);
			c->crypt(c, buf, VetEntraC + i);
			memcpy(C, VetEntraC + i, BLOCKS_BYTE);

			/* printf("VetEntra = " WFRT " " WFRT "\n", WORD(VetEntra + i), WORD(VetEntra + i + 8)); */
			/* printf("VetEntraC = " WFRT " " WFRT "\n", WORD(VetEntraC + i), WORD(VetEntraC + i + 8)); */
		}

		/* printf("\n\n"); */

		free(C);
		free(buf);
}

/*!
 * \brief Descriptografar array de bytes no modo CBC
 * \param c : Configurações da criptografia
 * \param num_blocks: número de bytes no VetEntraC
 * \param VetEntraC : array de bytes que deve ser descriptografado
 * \param VetEntra : array de bytes que conterá o resultado da descriptografia
 * \return void
*/
void
cbc_decode_array(CBC_Crypt *c, int num_blocks, w8* VetEntraC, w8* VetEntra) {
		int i;
		w8 *C, *buf;

		C = (w8*) malloc(BLOCKS_BYTE);
		buf = (w8*) malloc(BLOCKS_BYTE);

		assert(C != 0);
		assert(buf != 0);

		memset(C, 0xFF, BLOCKS_BYTE);

		for (i = 0; i < (num_blocks * BLOCKS_BYTE); i += BLOCKS_BYTE) {
			c->decrypt(c, VetEntraC + i, buf);
			xor_block(buf, C, VetEntra + i);
			memcpy(C, VetEntraC + i, BLOCKS_BYTE);

			printf("VetEntraC = " WFRT " " WFRT "\n", WORD(VetEntraC + i), WORD(VetEntraC + i + 8));
			printf("VetEntra = " WFRT " " WFRT "\n", WORD(VetEntra + i), WORD(VetEntra + i + 8));
		}

		printf("\n\n");

		free(C);
		free(buf);
}


void cbc_init(CBC_Crypt *c) {

	c->encode = &cbc_encode;
	c->decode = &cbc_decode;

}

void ebc_init(CBC_Crypt *c) {

	c->encode = &ebc_encode;
	c->decode = &ebc_decode;

}

void invert_word(w8 buf[BLOCKS_BYTE]) {

	int i, mid;
	w8 word;

	mid = (BLOCKS_BYTE >> 1);

	for (i = 0; i < mid; i++) {
		word = buf[i];
		buf[i] = buf[i + mid];
		buf[i + mid] = word;
	}

}

void ebc_encode(CBC_Crypt *cbc, char *filename, char *fileoutput) {
	FILE *in;
	FILE *out;

	LOG("Criptografia");

	in = fopen(filename, "rb+");
	out = fopen(fileoutput, "wb+");

	if (out == 0) {
		fprintf(stderr, "Erro ao abrir %s para escrita\n", fileoutput);
	} else if (in != 0) {
		w8 buf[BLOCKS_BYTE], buf_out[BLOCKS_BYTE];
		size_t sz;
		int i, ok;
		w64 save_size;
		int c = 1;

		memset(buf, 0x0, BLOCKS_BYTE);
		memset(buf_out, 0x0, BLOCKS_BYTE);

		fseek(in, 0, SEEK_END);
		save_size = (w64) ftell(in);
		fseek(in, 0, SEEK_SET);

		memcpy(buf, &save_size, sizeof(w64));
		invert_word(buf);

		cbc->crypt(cbc, buf, buf_out);
		fwrite(buf_out, BLOCKS_BYTE, 1, out);

		while (!feof(in)) {
			c++;
			memset(buf, 0x0, BLOCKS_BYTE);
			sz = 0;
			ok = 1;

			for (i = 0; (i < cbc->word_length) && ok; i++) {
				ok = fread(buf+i, 1, 1, in);
				sz += ok;
			}

			invert_word(buf);

			cbc->crypt(cbc, buf, buf_out);
			fwrite(buf_out, BLOCKS_BYTE, 1, out);

		}

		fclose(in);
		fclose(out);
	} else {
		fprintf(stderr, "Erro ao abrir o arquivo %s\n", filename);
	}
}

void
ebc_decode(CBC_Crypt *cbc, char *filename, char *fileoutput) {
	FILE *in;
	FILE *out;

	LOG("Descriptografia");

	in = fopen(filename, "rb+");
	out = fopen(fileoutput, "wb+");

	if (out == 0) {
		fprintf(stderr, "Erro ao abrir %s para escrita\n", fileoutput);
	} else if (in != 0) {
		w8 buf[BLOCKS_BYTE], buf_out[BLOCKS_BYTE];
		size_t sz;
		int i, ok;
		w64 save_size;
		int c = 1;

		fread(buf, BLOCKS_BYTE, 1, in);

		cbc->decrypt(cbc, buf, buf_out);
		invert_word(buf);

		memcpy(&save_size, buf_out, sizeof(w64));
		
		i = 0;
		while (fread(buf, BLOCKS_BYTE, 1, in) != 0) {

			cbc->decrypt(cbc, buf, buf_out);

			if ((i + cbc->word_length) > save_size) {
				fwrite(buf_out, save_size % cbc->word_length, 1, out);
			} else {
				i += cbc->word_length;
				fwrite(buf_out, cbc->word_length, 1, out);
			}

		}

		fclose(in);
		fclose(out);

	} else {

		fprintf(stderr, "Erro ao abrir o arquivo %s\n", filename);

	}
}

