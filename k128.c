
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "k128.h"

// Gerada com o arquivo calc_table_point.py
static w8 expr[256] = {1 , 45 , 226 , 147 , 190 , 69 , 21 , 174 , 120 , 3 , 135 , 164 , 184 , 56 , 207 , 63 , 8 , 103 , 9 , 148 , 235 , 38 , 168 , 107 , 189 , 24 , 52 , 27 , 187 , 191 , 114 , 247 , 64 , 53 , 72 , 156 , 81 , 47 , 59 , 85 , 227 , 192 , 159 , 216 , 211 , 243 , 141 , 177 , 255 , 167 , 62 , 220 , 134 , 119 , 215 , 166 , 17 , 251 , 244 , 186 , 146 , 145 , 100 , 131 , 241 , 51 , 239 , 218 , 44 , 181 , 178 , 43 , 136 , 209 , 153 , 203 , 140 , 132 , 29 , 20 , 129 , 151 , 113 , 202 , 95 , 163 , 139 , 87 , 60 , 130 , 196 , 82 , 92 , 28 , 232 , 160 , 4 , 180 , 133 , 74 , 246 , 19 , 84 , 182 , 223 , 12 , 26 , 142 , 222 , 224 , 57 , 252 , 32 , 155 , 36 , 78 , 169 , 152 , 158 , 171 , 242 , 96 , 208 , 108 , 234 , 250 , 199 , 217 , 0 , 212 , 31 , 110 , 67 , 188 , 236 , 83 , 137 , 254 , 122 , 93 , 73 , 201 , 50 , 194 , 249 , 154 , 248 , 109 , 22 , 219 , 89 , 150 , 68 , 233 , 205 , 230 , 70 , 66 , 143 , 10 , 193 , 204 , 185 , 101 , 176 , 210 , 198 , 172 , 30 , 65 , 98 , 41 , 46 , 14 , 116 , 80 , 2 , 90 , 195 , 37 , 123 , 138 , 42 , 91 , 240 , 6 , 13 , 71 , 111 , 112 , 157 , 126 , 16 , 206 , 18 , 39 , 213 , 76 , 79 , 214 , 121 , 48 , 104 , 54 , 117 , 125 , 228 , 237 , 128 , 106 , 144 , 55 , 162 , 94 , 118 , 170 , 197 , 127 , 61 , 175 , 165 , 229 , 25 , 97 , 253 , 77 , 124 , 183 , 11 , 238 , 173 , 75 , 34 , 245 , 231 , 115 , 35 , 33 , 200 , 5 , 225 , 102 , 221 , 179 , 88 , 105 , 99 , 86 , 15 , 161 , 49 , 149 , 23 , 7 , 58 , 40};
 
static w8 log[256] = {128 , 0 , 176 , 9 , 96 , 239 , 185 , 253 , 16 , 18 , 159 , 228 , 105 , 186 , 173 , 248 , 192 , 56 , 194 , 101 , 79 , 6 , 148 , 252 , 25 , 222 , 106 , 27 , 93 , 78 , 168 , 130 , 112 , 237 , 232 , 236 , 114 , 179 , 21 , 195 , 255 , 171 , 182 , 71 , 68 , 1 , 172 , 37 , 201 , 250 , 142 , 65 , 26 , 33 , 203 , 211 , 13 , 110 , 254 , 38 , 88 , 218 , 50 , 15 , 32 , 169 , 157 , 132 , 152 , 5 , 156 , 187 , 34 , 140 , 99 , 231 , 197 , 225 , 115 , 198 , 175 , 36 , 91 , 135 , 102 , 39 , 247 , 87 , 244 , 150 , 177 , 183 , 92 , 139 , 213 , 84 , 121 , 223 , 170 , 246 , 62 , 163 , 241 , 17 , 202 , 245 , 209 , 23 , 123 , 147 , 131 , 188 , 189 , 82 , 30 , 235 , 174 , 204 , 214 , 53 , 8 , 200 , 138 , 180 , 226 , 205 , 191 , 217 , 208 , 80 , 89 , 63 , 77 , 98 , 52 , 10 , 72 , 136 , 181 , 86 , 76 , 46 , 107 , 158 , 210 , 61 , 60 , 3 , 19 , 251 , 151 , 81 , 117 , 74 , 145 , 113 , 35 , 190 , 118 , 42 , 95 , 249 , 212 , 85 , 11 , 220 , 55 , 49 , 22 , 116 , 215 , 119 , 167 , 230 , 7 , 219 , 164 , 47 , 70 , 243 , 97 , 69 , 103 , 227 , 12 , 162 , 59 , 28 , 133 , 24 , 4 , 29 , 41 , 160 , 143 , 178 , 90 , 216 , 166 , 126 , 238 , 141 , 83 , 75 , 161 , 154 , 193 , 14 , 122 , 73 , 165 , 44 , 129 , 196 , 199 , 54 , 43 , 127 , 67 , 149 , 51 , 242 , 108 , 104 , 109 , 240 , 2 , 40 , 206 , 221 , 155 , 234 , 94 , 153 , 124 , 20 , 134 , 207 , 229 , 66 , 184 , 64 , 120 , 45 , 58 , 233 , 100 , 31 , 146 , 144 , 125 , 57 , 111 , 224 , 137 , 48};

/*!
 * \brief Converte uma sequência de 8 bytes de um array em palavra de 64 bits
 * \param buffer : Array que contém a sequência
 * \param n : Posição do primeiro elemento da sequência a ser convertida
 * \return Palavra resultante da conversão
 */
static inline 
w64 to_uint64(w8* buffer, int n) {
	return (
	(w64)buffer[n] << 56 | (w64)buffer[n+1] << 48 | 
	(w64)buffer[n+2] << 40  | (w64)buffer[n+3] << 32 | 
	(w64) buffer[n+4] << 24 | (w64)buffer[n+5] << 16 |
	(w64)buffer[n+6] << 8  | (w64)buffer[n+7]);
}

static inline void 
uint64_to_bytes(w64 in, int n, w8* out) {
	int i;

	for (i = 7; i >= 0; i--) {
		out[i + n] = in & 0xFF;
		in = in >> 8;
	}
}

/*!
 * \brief Deslocamento circular para esquerda
 * \param a : Palavra de 64 bits
 * \param b : Tamanho do deslocamento
 * \return Resultado do deslocamento
 */
static inline w64
shift_circ_left(w64 a, int b) {
	b = b % 64;
	if (b == 0)
		return a;
	return (a << b) | (a >> (64 - b));
}

static inline w64
point(w64 b, w64 c) { // A = B . C
	w8 a_bytes[8];
	w8 b_bytes[8];
	w8 c_bytes[8];
	int i;

	uint64_to_bytes(b, 0, b_bytes);
	uint64_to_bytes(c, 0, c_bytes);

	for (i = 0; i < 8; i++) {
		a_bytes[i] = expr[b_bytes[i]] ^ expr[c_bytes[i]];
	}

	return to_uint64(a_bytes, 0);
}

static inline w64
point_inverse_left(w64 a, w64 c) { // A = B . C => B = A . C
	w8 a_bytes[8];
	w8 b_bytes[8];
	w8 c_bytes[8];
	int i;

	uint64_to_bytes(a, 0, a_bytes);
	uint64_to_bytes(c, 0, c_bytes);

	for (i = 0; i < 8; i++) {
		b_bytes[i] = log[ a_bytes[i] ^ expr[c_bytes[i]] ];
	}

	return to_uint64(b_bytes, 0);
}

/*!
 * \brief Gera as subchaves do K128
 * \param key: string da chave terminada com '\0'
 * \param k: parâmetro de saída que contém as NSUBKEYS subchaves
 * \return void
*/
void
generate_keys(char *key, w64** k) {

	w64 *L, *kl, A, B;
	int i, j, lenkey, s;
	char newkey[16];

	lenkey = strlen(key);

	if (lenkey >= 16) {
		memcpy(newkey, key, 16);
	} else {
		for (i = 0; i < 16; i++) {
			newkey[i] = key[(i % lenkey)];
		}
	}

	*k = kl = malloc(NSUBKEYS * sizeof(w64));
	L = malloc((NSUBKEYS + 1) * sizeof(w64));

	L[0] = to_uint64(newkey, 0);
	L[1] = to_uint64(newkey, 8);

#if DEBUG
	printf("newkey = ");
	for (i = 0; i < 16; i++) {
		printf("%c", newkey[i]);
	}
	printf("\n");
#endif
	
	for (j = 2; j <= NSUBKEYS; j++) {
		L[j] = L[j-1] + 0x9e3779b97f4a7c15;
	}

	kl[0] = 0x8aed2a6bb7e15162;

	for (j = 1; j <= NSUBKEYS; j++) {
		kl[j] = kl[j-1] + 0x7c159e3779b97f4a;
	}

	i = j = A = B = 0;

#if DEBUG
	printf("NSUBKEYS = %d\n", NSUBKEYS);
#endif

	for (s = 1; s <= (NSUBKEYS+1); s++) {
		kl[i] = shift_circ_left(kl[i] + A + B, 3);
		A = kl[i];
		i = i + 1;

		L[j] = shift_circ_left(L[j] + A + B, A + B);
		B = L[j];
		j = j + 1;
	}

	//free(L);
}

// Primeira parte de uma iteração do K128
static inline void
encode_first_iteration(w64 Xa, w64 Xb, w64 *skeys, w8 it, w64 *X_new_a, w64 *X_new_b) {

	assert(X_new_a != 0);
	assert(X_new_b != 0);
	assert(skeys != 0);

	w64 Ka = skeys[it];
	w64 Kb = skeys[it+1];
	
	*X_new_a = point(Xa, Ka);
	*X_new_b = Xb + Kb;
}

// Inversa da primeira parte de um iteração do K128
static inline void
encode_first_iteration_inverse(w64 Xa, w64 Xb, w64 *skeys, w8 it, w64 *X_new_a, w64 *X_new_b) {

	assert(X_new_a != 0);
	assert(X_new_b != 0);
	assert(skeys != 0);

	w64 Ka = skeys[it];
	w64 Kb = skeys[it+1];

	*X_new_a = point_inverse_left(Xa, Ka);
	*X_new_b = Xb + (~Kb) + 1;
}

// Segunda parte de uma iteração do K128
static inline void
encode_second_iteration(w64 Xe, w64 Xf, w64 *skeys, w8 it, w64 *X_new_e, w64 *X_new_f) {

	assert(X_new_e != 0);
	assert(X_new_f != 0);
	assert(skeys != 0);

	w64 Ke = skeys[it+2];
	w64 Kf = skeys[it+3];
	w64 Y1, Y2, Z;

	Y1 = Xe ^ Xf;

	Y2 = point((point(Ke, Y1) + Y1), Kf);
	Z = point(Ke, Y1) + Y2;
	
	*X_new_e = Z ^ Xe;
	*X_new_f = Z ^ Xf;
}

// Inversa da segunda parte de uma iteração do K128
static inline void
encode_second_iteration_inverse(w64 Xe, w64 Xf, w64 *skeys, w8 it, w64 *X_new_e, w64 *X_new_f) {

	assert(X_new_e != 0);
	assert(X_new_f != 0);
	assert(skeys != 0);

	encode_second_iteration(Xe, Xf, skeys, it, X_new_e, X_new_f);
}

// Última iteração do K128
static inline void
encode_last_iteration(w64 Xe, w64 Xf, w64 *skeys, w64 *X_e_final, w64 *X_f_final) {

	assert(X_e_final != 0);
	assert(X_f_final != 0);
	assert(skeys != 0);

	*X_e_final = point(Xf, skeys[(NSUBKEYS << 2) + 1]);
	*X_f_final = Xe + skeys[(NSUBKEYS << 2) + 2];
}

// Inversa da última iteração do K128
static inline void
encode_last_iteration_inverse(w64 Xe, w64 Xf, w64 *skeys, w64 *X_e_final, w64 *X_f_final) {

	assert(X_e_final != 0);
	assert(X_f_final != 0);
	assert(skeys != 0);

	
	*X_f_final = point_inverse_left(Xe, skeys[(NSUBKEYS << 2) + 1]);
	*X_e_final = Xf + (~skeys[(NSUBKEYS << 2) + 2]) + 1;
}
 
void
print_hex(w8 *buf, int sz) {
	int i;
	for (i = 0; i < sz; i++) {
		printf("0x%x ", buf[i]);
	}
}

/*!
 * \brief Função de criptografia do K128
 * \param in : Entrada descriptografada
 * \param out : Saída criptografada com K128
 * \param skeys : Subchaves do K128
 * \return void
*/
void
k128_encode(w8 in[16], w8 out[16], w64 *skeys) {

	int i;

	assert(skeys != 0);
	
	w64 A = to_uint64(in, 0), new_A, new_B;
	w64 B = to_uint64(in, 8);

	LOG("L = 0x%llx 0x%llx", A, B);

	for (i = 1; i <= ROUNDS; i++) {
		encode_first_iteration(A, B, skeys, i, &new_A, &new_B);
		LOG("1(%d) = 0x%llx 0x%llx", i, new_A, new_B);

		encode_second_iteration(new_A, new_B, skeys, i, &A, &B);
		LOG("2(%d) = 0x%llx 0x%llx", i, A, B);
	}

	encode_last_iteration(A, B, skeys, &new_A, &new_B);
	uint64_to_bytes(new_A, 0, out);
	uint64_to_bytes(new_B, 8, out);

	LOG("L = 0x%llx 0x%llx", new_A, new_B);
}

/*!
 * \brief Função de descriptografia do K128
 * \param in : Entrada criptografada com K128
 * \param out : Saída descriptografada
 * \param skeys : Subchaves do K128
 * \return void
*/
void
k128_decode(w8 in[16], w8 out[16], w64 *skeys) {

	int i;

	w64 A = to_uint64(in, 0), new_A, new_B;
	w64 B = to_uint64(in, 8);

	encode_last_iteration_inverse(A, B, skeys, &new_A, &new_B);
	uint64_to_bytes(new_A, 0, out);
	uint64_to_bytes(new_B, 8, out);

	LOG("\n\n-LI = 0x%llx 0x%llx", new_A, new_B);

	A = new_A;
	B = new_B;

	for (i = ROUNDS; i >= 1; i--) {

		encode_second_iteration_inverse(A, B, skeys, i, &new_A, &new_B);
		LOG("-2(%d) = 0x%llx 0x%llx", i, new_A, new_B);

		encode_first_iteration_inverse(new_A, new_B, skeys, i, &A, &B);
		LOG("-1(%d) = 0x%llx 0x%llx", i, A, B);

	}

	uint64_to_bytes(A, 0, out);
	uint64_to_bytes(B, 8, out);

	LOG("plain = 0x%llx 0x%llx", A, B);
}
/*!
 * \brief XOR dos arrays 'a' e 'b' e coloca o array resultante em 'result'
 * \param a : Operando do xor
 * \param b : Operando do xor
 * \param result : Resultado do xor
 * \return void
*/
inline static void
xor_block (w8 a[BLOCKS_BYTE], w8 b[BLOCKS_BYTE], w8 result[BLOCKS_BYTE]) {
	int i;

	for (i = 0; i < BLOCKS_BYTE; i++) {
		result[i] = a[i] ^ b[i];
	}
}

/*!
 * \brief Função de criptografia do K128 para ser usada no CBC
 * \param c : Configurações da criptografia
 * \param in : Entrada descriptografada
 * \param out : Saída criptografada com K128
 * \return void
*/
static void
k128_encode_cbc(CBC_Crypt *c, w8 in[16], w8 out[16]) {
	int i;
	assert(c != 0);

	struct __K128_Crypt *k = (struct __K128_Crypt*) c;
	k128_encode(in, out, k->__skeys);

}

/*!
 * \brief Função de descriptografia do K128 para ser usada no CBC
 * \param c : Configurações da criptografia
 * \param in : Entrada criptografada com o K128
 * \param out : Saída descriptografada
 * \return void
*/
static void
k128_decode_cbc(CBC_Crypt *c, w8 in[16], w8 out[16]) {
	int i;
	assert(c != 0);

	struct __K128_Crypt *k = (struct __K128_Crypt*) c;
	k128_decode(in, out, k->__skeys);
}

/*!
 * \brief Inicializa os parâmetros para usar a criptografia do K128
 * \param c : Criptografia
 * \param mainkey : Chave da criptografia
 * \return void
*/
void
k128_init(CBC_Crypt *c, char *mainkey) {
	assert(c != 0);
	assert(mainkey != 0);

	struct __K128_Crypt *k = (struct __K128_Crypt*) c;

	c->crypt = &k128_encode_cbc;
	c->decrypt = &k128_decode_cbc;

	generate_keys(mainkey, &k->__skeys);
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

		// O primeiro bloco deve conter o tamanho do arquivo
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
			cbc->crypt(cbc, buf_out, buf); // C' = crypt(P ^ C)
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

		// o primeiro bloco contém o tamanho do arquivo
		memcpy(C_new, buf, BLOCKS_BYTE);
		cbc->decrypt(cbc, buf, buf_out);
		xor_block(buf_out, C, buf);
		save_size = to_uint64(buf, 0);
		//xor_block(buf, C, C_new);
		memcpy(C, C_new, BLOCKS_BYTE);
		
		LOG("P[%d] = " WFRT " " WFRT, c, WORD(buf), WORD(buf + 8));

		i = 0;
		while (fread(buf, BLOCKS_BYTE, 1, in) != 0) { // = C'
			c++;
			LOG("C[%d] = " WFRT " " WFRT, c, WORD(buf), WORD(buf + 8));

			memcpy(C_new, buf, BLOCKS_BYTE);
			cbc->decrypt(cbc, buf, buf_out); // decrypt(C')
			LOG("D(C[%d]) = " WFRT " " WFRT, c, WORD(buf_out), WORD(buf_out + 8));
			xor_block(buf_out, C, buf); // decrypt(C') ^ C
			LOG("C = " WFRT " " WFRT, c, WORD(C), WORD(C + 8));
			LOG("D(C[%d]) ^ C = " WFRT " " WFRT, c, WORD(buf), WORD(buf + 8));
			if ((i + BLOCKS_BYTE) > save_size) {
				fwrite(buf, save_size % BLOCKS_BYTE, 1, out);
			} else {
				i += BLOCKS_BYTE;
				fwrite(buf, BLOCKS_BYTE, 1, out); // P = decrypt(C') ^ C
			}
			//xor_block(buf, C, C_new);
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

			//printf("VetEntra = " WFRT " " WFRT "\n", WORD(VetEntra + i), WORD(VetEntra + i + 8));
			//printf("VetEntraC = " WFRT " " WFRT "\n", WORD(VetEntraC + i), WORD(VetEntraC + i + 8));
		}

		//printf("\n\n");

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

/*!
 * \brief Calcula a distância de Hamming
 * \param x : Array de bytes
 * \param y : Array de bytes
 * \param size : Tamanho dos arrays
 * \return Retorna a distância de Hamming
*/
static inline int
hamming_distance(w8 *x, w8 *y, size_t size) {
	int i, res;
	w8 z;

	res = 0;

	for (i = 0; i < size; i++) {
		z = x[i] ^ y[i];
		res += __builtin_popcount(z);
	}

	return res;
}

/*!
 * \brief Calcula a aleatoriedade do k128 de acordo com o parâmetro 'mode'
 * \param filename: Arquivo que fornecerá os 1024 bits para teste
 * \param password: Palavra-chave utilizada no teste
 * \param mode: Modo para medir aleatoriedade. Se mode = 1, altera somente os j-ésimo bit; caso mode = 2, então altera j-ésimo e o (j+8)-ésimo bits
 * \return void
*/
void
randomness_k128_mode(char *filename, char *password, int mode) {
	FILE *file;
	w8* VetEntra, *VetEntraC, *VetAlter, *VetAlterC;
	CBC_Crypt c;
	int ok = 1, num_blocks = 8, j, byte, block, byte_block, bit_block,
	 j1, byte1, byte_block1, bit_block1, block1, limit;
	int H_max[8], H_min[8], H_mean[8], i, tmp;
	w8 bits;

	VetEntra = (w8*) malloc(num_blocks * BLOCKS_BYTE);
	VetEntraC = (w8*) malloc(num_blocks * BLOCKS_BYTE);	
	VetAlter = (w8*) malloc(num_blocks * BLOCKS_BYTE);
	VetAlterC = (w8*) malloc(num_blocks * BLOCKS_BYTE);

	assert(VetEntra != 0);
	assert(VetEntraC != 0);
	assert(VetAlter != 0);
	assert(VetAlterC != 0);
	assert(mode >= 1 && mode <= 2);

	file = fopen(filename, "rb+");

	if (file == 0) {
		fprintf(stderr, "Erro ao abrir o arquivo %s\n", filename);
	} else {

		ok = fread(VetEntra, BLOCKS_BYTE, num_blocks, file);	
		fclose(file);

		if (!ok) {
			fprintf(stderr, "O arquivo possui não mais que %d bits", BLOCKS_BYTE * num_blocks);
			return;
		}
	
		k128_init(&c, password);

		cbc_encode_array(&c, num_blocks, VetEntra, VetEntraC);

		for (j = 0; j < 8; j++) {
			H_max[j] = 0;
			H_min[j] = 1000;
			H_mean[j] = 0;
		}

		limit = (BLOCKS_BYTE * num_blocks * 8 - (8 * (mode == 2)));

		for (j = 0; j < limit; j++) {
			byte = j / 8;
			block = byte / BLOCKS_BYTE;
			byte_block = byte % BLOCKS_BYTE;
			bit_block = j - (BLOCKS_BYTE*block*8 + byte_block*8);

			memcpy(VetAlter, VetEntra, num_blocks * BLOCKS_BYTE);

			*(VetAlter + (block * BLOCKS_BYTE) + byte_block) =
				*(VetEntra + (block * BLOCKS_BYTE) + byte_block) ^ ((w8) (1 << bit_block));

			if (mode == 2) {
				j1 = j + 8;
				byte1 = j1 / 8;
				block1 = byte1 / BLOCKS_BYTE;
				byte_block1 = byte1 % BLOCKS_BYTE;
				bit_block1 = j1 - (BLOCKS_BYTE*block1*8 + byte_block1*8);

				*(VetAlter + (block1 * BLOCKS_BYTE) + byte_block1) =
				 (*(VetAlter + (block1 * BLOCKS_BYTE) + byte_block1)) ^ ((w8) (1 << bit_block1));
			}

			cbc_encode_array(&c, num_blocks, VetAlter, VetAlterC);
			
			for (i = block; i < num_blocks; i++) {
				tmp = hamming_distance((VetEntraC + (i * BLOCKS_BYTE)),
				 (VetAlterC + (i * BLOCKS_BYTE)), BLOCKS_BYTE);

				H_max[i] = (tmp > H_max[i]) ? tmp : H_max[i];
				H_min[i] = (tmp < H_min[i]) ? tmp : H_min[i];
				H_mean[i] += tmp;
			}
		}

		printf(" i | H máx. | H min. | H média \n");
		printf("-------------------------------\n");

		for (j = 0; j < 8; j++) {
			printf(" %d | %6d | %6d | %3d \n", j, H_max[j], H_min[j], H_mean[j] / ((j + 1) * BLOCKS_BYTE * 8));
			printf("-------------------------------\n");
		}


	}

	free(VetEntra);
	free(VetEntraC);
	free(VetAlter);
	free(VetAlterC);
}

static inline void _test_cbc_generic(void) {
	CBC_Crypt c;

	k128_init(&c, "Exemplo01");
}

static inline void _test_generate_keys(void) {

	w64 k[51];
	int i;
	w64 *keys;
	char *ex = "Exemplo01";

	generate_keys(ex, &keys);

	k[1] = 0x58f89b84a7794edfLL;
	k[2] = 0x3663465cd9367d4LL;
	k[3] = 0xec15ba6ab71cea87LL;
	k[4] = 0x68f3f3c4884f1f98LL;
	k[5] = 0x5be49705c5800c0LL;
	k[6] = 0x8180f16d92e487c2LL;
	k[7] = 0x28754cc74732010fLL;
	k[8] = 0xc22ec96b457e4d0aLL;
	k[9] = 0x62519fb807ecf96eLL;
	k[10] = 0x46e38c41b511dc34LL;
	k[11] = 0xf97ee520aa9793b6LL;
	k[12] = 0x6ca919bbbd19b075LL;
	k[13] = 0xf7eb0562a5f4923cLL;
	k[14] = 0xbd61747b389c84d1LL;
	k[15] = 0x9e6e518071c1b062LL;
	k[16] = 0x767b755b212fc6f0LL;
	k[17] = 0x6238dd2d6f1363fLL;
	k[18] = 0xf2f155d98ebd0e43LL;
	k[19] = 0xb17d4f86c81ec4d3LL;
	k[20] = 0x6927488db216b893LL;
	k[21] = 0xa820a2efd85fe267LL;
	k[22] = 0x282ab572d3e68766LL;
	k[23] = 0x969df2ecf6304a2cLL;
	k[24] = 0xb931c5fe9274a3f4LL;
	k[25] = 0x3555a2638cb05423LL;
	k[26] = 0x645befb304458f9dLL;
	k[27] = 0xb8313a41069e7bd4LL;
	k[28] = 0x74c462bf720e0b9fLL;
	k[29] = 0xf16973358861784fLL;
	k[30] = 0xe75a09fcf79cb3c5LL;
	k[31] = 0x22b36e893f46a503LL;
	k[32] = 0x5719b7c63ec1d4bdLL;
	k[33] = 0x9126e95c4f3f3425LL;
	k[34] = 0x8d1a2f55d0ef4357LL;
	k[35] = 0x2032efb5d676b117LL;
	k[36] = 0x1f0122da9f8df2b8LL;
	k[37] = 0x1f6b097b8e0785c7LL;
	k[38] = 0xb22458503266fe13LL;
	k[39] = 0xc25faa5c7b3418dLL;
	k[40] = 0x87889c3b8bf71805LL;
	k[41] = 0xd0c48e3721eb31bLL;
	k[42] = 0x2dd7520825fc1fcLL;
	k[43] = 0xf374fada26471ca9LL;
	k[44] = 0xfb49115155d44000LL;
	k[45] = 0xbd4823261fff4855LL;
	k[46] = 0x412cfc62e966c81eLL;
	k[47] = 0x5a5e606cbf353d79LL;
	k[48] = 0x719edcfef7e58083LL;
	k[49] = 0x80438808f9e0eb1cLL;
	k[50] = 0x89c6449b615c0976LL;

	for (i = 1; i <= NSUBKEYS; i++) {
		assert(k[i] == keys[i]);
	}

	printf("generate_keys OK\n");
}

static inline void
_test_convert() {
	w8 bytes[8] = {1 , 45 , 226 , 147 , 190 , 69 , 21 , 174};
	w8 bytes1[8] = {1 , 45 , 226 , 147 , 190 , 69 , 21 , 174};
	w64 x;
	int i;

	x = to_uint64(bytes, 0);
	uint64_to_bytes(x, 0, bytes);

	for (i = 0; i < 8; i++) {
		assert(bytes1[i] == bytes[i]);
	}

	printf("_test_convert OK\n");
}

_test_k128_encode() {


	{
		w64 *keys;
		char *sec = "Exemplo01";
		w8 in[16] = {44,226,147,190,69,21,122,111,174,133,120,23,123,142,31,24}, out[16], out1[16];

		generate_keys(sec, &keys);
	
		k128_encode(in, out, keys);
		k128_decode(out, out1, keys);

		assert(memcmp(in, out1, 16) == 0);

	}


	{
		w64 *keys;
		char *sec = "Exemplo01";
		w8 in[16] = {31,32,33,34,35,36,37,38,31,32,33,34,35,36,37,38}, out[16], out1[16];

		generate_keys(sec, &keys);
	
		k128_encode(in, out, keys);
		k128_decode(out, out1, keys);

		assert(memcmp(in, out1, 16) == 0);
	}
}

void tests_functions(void) {

	_test_generate_keys();
	_test_convert();
	_test_k128_encode();
	_test_cbc_generic();
}
