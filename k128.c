
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "k128.h"

static w8 expr[256] = {1 , 45 , 226 , 147 , 190 , 69 , 21 , 174 , 120 , 3 , 135 , 164 , 184 , 56 , 207 , 63 , 8 , 103 , 9 , 148 , 235 , 38 , 168 , 107 , 189 , 24 , 52 , 27 , 187 , 191 , 114 , 247 , 64 , 53 , 72 , 156 , 81 , 47 , 59 , 85 , 227 , 192 , 159 , 216 , 211 , 243 , 141 , 177 , 255 , 167 , 62 , 220 , 134 , 119 , 215 , 166 , 17 , 251 , 244 , 186 , 146 , 145 , 100 , 131 , 241 , 51 , 239 , 218 , 44 , 181 , 178 , 43 , 136 , 209 , 153 , 203 , 140 , 132 , 29 , 20 , 129 , 151 , 113 , 202 , 95 , 163 , 139 , 87 , 60 , 130 , 196 , 82 , 92 , 28 , 232 , 160 , 4 , 180 , 133 , 74 , 246 , 19 , 84 , 182 , 223 , 12 , 26 , 142 , 222 , 224 , 57 , 252 , 32 , 155 , 36 , 78 , 169 , 152 , 158 , 171 , 242 , 96 , 208 , 108 , 234 , 250 , 199 , 217 , 0 , 212 , 31 , 110 , 67 , 188 , 236 , 83 , 137 , 254 , 122 , 93 , 73 , 201 , 50 , 194 , 249 , 154 , 248 , 109 , 22 , 219 , 89 , 150 , 68 , 233 , 205 , 230 , 70 , 66 , 143 , 10 , 193 , 204 , 185 , 101 , 176 , 210 , 198 , 172 , 30 , 65 , 98 , 41 , 46 , 14 , 116 , 80 , 2 , 90 , 195 , 37 , 123 , 138 , 42 , 91 , 240 , 6 , 13 , 71 , 111 , 112 , 157 , 126 , 16 , 206 , 18 , 39 , 213 , 76 , 79 , 214 , 121 , 48 , 104 , 54 , 117 , 125 , 228 , 237 , 128 , 106 , 144 , 55 , 162 , 94 , 118 , 170 , 197 , 127 , 61 , 175 , 165 , 229 , 25 , 97 , 253 , 77 , 124 , 183 , 11 , 238 , 173 , 75 , 34 , 245 , 231 , 115 , 35 , 33 , 200 , 5 , 225 , 102 , 221 , 179 , 88 , 105 , 99 , 86 , 15 , 161 , 49 , 149 , 23 , 7 , 58 , 40};
 
static w8 log[256] = {128 , 0 , 176 , 9 , 96 , 239 , 185 , 253 , 16 , 18 , 159 , 228 , 105 , 186 , 173 , 248 , 192 , 56 , 194 , 101 , 79 , 6 , 148 , 252 , 25 , 222 , 106 , 27 , 93 , 78 , 168 , 130 , 112 , 237 , 232 , 236 , 114 , 179 , 21 , 195 , 255 , 171 , 182 , 71 , 68 , 1 , 172 , 37 , 201 , 250 , 142 , 65 , 26 , 33 , 203 , 211 , 13 , 110 , 254 , 38 , 88 , 218 , 50 , 15 , 32 , 169 , 157 , 132 , 152 , 5 , 156 , 187 , 34 , 140 , 99 , 231 , 197 , 225 , 115 , 198 , 175 , 36 , 91 , 135 , 102 , 39 , 247 , 87 , 244 , 150 , 177 , 183 , 92 , 139 , 213 , 84 , 121 , 223 , 170 , 246 , 62 , 163 , 241 , 17 , 202 , 245 , 209 , 23 , 123 , 147 , 131 , 188 , 189 , 82 , 30 , 235 , 174 , 204 , 214 , 53 , 8 , 200 , 138 , 180 , 226 , 205 , 191 , 217 , 208 , 80 , 89 , 63 , 77 , 98 , 52 , 10 , 72 , 136 , 181 , 86 , 76 , 46 , 107 , 158 , 210 , 61 , 60 , 3 , 19 , 251 , 151 , 81 , 117 , 74 , 145 , 113 , 35 , 190 , 118 , 42 , 95 , 249 , 212 , 85 , 11 , 220 , 55 , 49 , 22 , 116 , 215 , 119 , 167 , 230 , 7 , 219 , 164 , 47 , 70 , 243 , 97 , 69 , 103 , 227 , 12 , 162 , 59 , 28 , 133 , 24 , 4 , 29 , 41 , 160 , 143 , 178 , 90 , 216 , 166 , 126 , 238 , 141 , 83 , 75 , 161 , 154 , 193 , 14 , 122 , 73 , 165 , 44 , 129 , 196 , 199 , 54 , 43 , 127 , 67 , 149 , 51 , 242 , 108 , 104 , 109 , 240 , 2 , 40 , 206 , 221 , 155 , 234 , 94 , 153 , 124 , 20 , 134 , 207 , 229 , 66 , 184 , 64 , 120 , 45 , 58 , 233 , 100 , 31 , 146 , 144 , 125 , 57 , 111 , 224 , 137 , 48};
 
static inline w64 to_uint64(w8* buffer, int n) {
	return (
	(w64)buffer[n] << 56 | (w64)buffer[n+1] << 48 | 
	(w64)buffer[n+2] << 40  | (w64)buffer[n+3] << 32 | 
	(w64) buffer[n+4] << 24 | (w64)buffer[n+5] << 16 |
	(w64)buffer[n+6] << 8  | (w64)buffer[n+7]);
}

static inline void uint64_to_bytes(w64 in, int n, w8* out) {
	int i;

	for (i = 7; i >= 0; i--) {
		out[i + n] = in & 0xFF;
		in = in >> 8;
	}
}

w64
shift_circ_left(w64 a, int b) {
	b = b % 64;
	if (b == 0)
		return a;
	return (a << b) | (a >> (64 - b));
}

w64 point(w64 b, w64 c) {
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

w64 point_inverse_left(w64 a, w64 c) {
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

/*
 * @brief Gera as subchaves
 * @param key: string da chave terminada com '\0'
 * @param k: parâmetro de saída que contém as NSUBKEYS subchaves
 * @return void
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

void
encode_first_iteration(w64 Xa, w64 Xb, w64 *skeys, w8 it, w64 *X_new_a, w64 *X_new_b) {

	assert(X_new_a != 0);
	assert(X_new_b != 0);
	assert(skeys != 0);

	w64 Ka = skeys[it];
	w64 Kb = skeys[it+1];
	
	*X_new_a = point(Xa, Ka);
	*X_new_b = Xb + Kb;
}

void
encode_second_iteration(w64 Xe, w64 Xf, w64 *skeys, w8 it, w64 *X_new_e, w64 *X_new_f) {

	assert(X_new_e != 0);
	assert(X_new_f != 0);
	assert(skeys != 0);

	w64 Ke = skeys[it+2];
	w64 Kf = skeys[it+3];
	w64 Y1, Z1, Y2, Z2;

	Y1 = Xe ^ Xf;
	Z1 = Xe + Xf;

	Y2 = point((point(Xe, Y1) + Z1), Kf);
	Z2 = point(Ke, Y1) + Y2;
	
	*X_new_e = Xe + Y2;
	*X_new_f = Xf + Z2;
}

void
encode_second_iteration_inverse(w64 Xe, w64 Xf, w64 *skeys, w8 it, w64 *X_new_e, w64 *X_new_f) {

	assert(X_new_e != 0);
	assert(X_new_f != 0);
	assert(skeys != 0);

	w64 Ke = skeys[it+2];
	w64 Kf = skeys[it+3];
	w64 Y1, Z1, Y2, Z2;

	Y1 = Xe ^ Xf;
	Z1 = Xe + Xf;

	Y2 = point((point(Xe, Y1) + Z1), Kf);
	Z2 = point(Ke, Y1) + Y2;
	
	*X_new_e = Xe + Y2;
	*X_new_f = Xf + Z2;
}

void
encode_last_iteration(w64 Xe, w64 Xf, w64 *skeys, w64 *X_e_final, w64 *X_f_final) {

	assert(X_e_final != 0);
	assert(X_f_final != 0);
	assert(skeys != 0);

	*X_e_final = point(Xf, skeys[(NSUBKEYS << 2) + 1]);
	*X_f_final = Xe + skeys[(NSUBKEYS << 2) + 2];
}

void
encode_last_iteration_inverse(w64 Xe, w64 Xf, w64 *skeys, w64 *X_e_final, w64 *X_f_final) {

	assert(X_e_final != 0);
	assert(X_f_final != 0);
	assert(skeys != 0);

	
	*X_f_final = point_inverse_left(Xe, skeys[(NSUBKEYS << 2) + 1]);
	*X_e_final = Xf + (~skeys[(NSUBKEYS << 2) + 2]);
}
 
void
print_hex(w8 *buf, int sz) {
	int i;
	for (i = 0; i < sz; i++) {
		printf("0x%x ", buf[i]);
	}
}

void
k128_encode(w8 in[16], w8 out[16], w64 *skeys) {

	int i;

	assert(skeys != 0);
	
	w64 A = to_uint64(in, 0), new_A, new_B;
	w64 B = to_uint64(in, 8);

	printf("IN = "); print_hex(in, 16); printf("\n");

	for (i = 1; i <= ROUNDS; i++) {
		encode_first_iteration(A, B, skeys, i, &new_A, &new_B);

		uint64_to_bytes(new_A, 0, out);
		uint64_to_bytes(new_B, 8, out);
		printf("F[%d] = ", i); print_hex(out, 16); printf("\n");

		encode_second_iteration(new_A, new_B, skeys, i, &A, &B);

		uint64_to_bytes(A, 0, out);
		uint64_to_bytes(B, 8, out);
		printf("S[%d] = ", i); print_hex(out, 16); printf("\n");
	}

	encode_last_iteration(A, B, skeys, &new_A, &new_B);
	uint64_to_bytes(new_A, 0, out);
	uint64_to_bytes(new_B, 8, out);

	printf("LI = "); print_hex(out, 16); printf("\n\n\n");
}

void
k128_decode(w8 in[16], w8 out[16], w64 *skeys) {

	w64 A = to_uint64(in, 0), new_A, new_B;
	w64 B = to_uint64(in, 8);

	encode_last_iteration_inverse(A, B, skeys, &new_A, &new_B);
	uint64_to_bytes(new_A, 0, out);
	uint64_to_bytes(new_B, 8, out);

	printf("LI(-1) = "); print_hex(out, 16); printf("\n\n\n");
}

void
cbc_crypt(w8 in[BLOCKS_BYTE], w8 out[BLOCKS_BYTE]) {
	int i;

	for (i = 0; i < BLOCKS_BYTE; i++) {
		out[i] = expr[in[BLOCKS_BYTE - i - 1] ^ 0x2F];
		out[i] = ((out[i] >> 4) & 0xFF) + ((out[i] & 0xFF) << 4);
	}
}

void
cbc_decrypt(w8 in[BLOCKS_BYTE], w8 out[BLOCKS_BYTE]) {
	int i;

	for (i = 0; i < BLOCKS_BYTE; i++) {
		out[i] = in[BLOCKS_BYTE - i - 1];
		out[i] = ((out[i] >> 4) & 0xFF) + ((out[i] & 0xFF) << 4);
		out[i] = log[out[i]] ^ 0x2F;
	}
}

inline static void
xor_block (w8 a[BLOCKS_BYTE], w8 b[BLOCKS_BYTE], w8 result[BLOCKS_BYTE]) {
	int i;

	for (i = 0; i < BLOCKS_BYTE; i++) {
		result[i] = a[i] ^ b[i];
	}
}

void
cbc_encode(char *filename, char *fileoutput) {
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

		memset((void*) &C, 0xFF, BLOCKS_BYTE);

		fseek(in, 0, SEEK_END);
		save_size = (w64) ftell(in);
		fseek(in, 0, SEEK_SET);

		uint64_to_bytes(save_size, 0, buf);

		LOG("P[%d] = " WFRT " " WFRT, c, WORD(buf), WORD(buf + 8));

		// O primeiro bloco deve conter o tamanho do arquivo
		xor_block(buf, C, buf_out);
		cbc_crypt(buf_out, buf);
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
			cbc_crypt(buf_out, buf); // C' = crypt(P ^ C)
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

void
cbc_decode(char *filename, char *fileoutput) {
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

		memset((void*) &C, 0xFF, BLOCKS_BYTE);

		fread(buf, BLOCKS_BYTE, 1, in);

		LOG("C[%d] = " WFRT " " WFRT, c, WORD(buf), WORD(buf + 8));

		// o primeiro bloco contém o tamanho do arquivo
		memcpy(C_new, buf, BLOCKS_BYTE);
		cbc_decrypt(buf, buf_out);
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
			cbc_decrypt(buf, buf_out); // decrypt(C')
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
	w64 *keys;
	char *sec = "Exemplo01";
	w8 in[16] = {44,226,147,190,69,21,122,111,174,133,120,23,123,142,31,24}, out[16];

	generate_keys(sec, &keys);

	k128_encode(in, out, keys);
	k128_decode(out, in, keys);
}

void tests_functions(void) {

	_test_generate_keys();
	_test_convert();
	_test_k128_encode();

}
