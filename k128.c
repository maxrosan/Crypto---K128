
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define w64 unsigned long long int
#define w32 unsigned int
#define w8 unsigned char


#define ROUNDS 12
#define NSUBKEYS (4*ROUNDS + 2)
#define DEBUG 1
#define to_uint64(buffer,n) ((w64)buffer[n] << 56 | (w64)buffer[n+1] << 48 | (w64)buffer[n+2] << 40  | (w64)buffer[n+3] << 32 | (w64) buffer[n+4] << 24 | (w64)buffer[n+5] << 16 | (w64)buffer[n+6] << 8  | (w64)buffer[n+7])

static w8 expr[256] = {1 , 45 , 226 , 147 , 190 , 69 , 21 , 174 , 120 , 3 , 135 , 164 , 184 , 56 , 207 , 63 , 8 , 103 , 9 , 148 , 235 , 38 , 168 , 107 , 189 , 24 , 52 , 27 , 187 , 191 , 114 , 247 , 64 , 53 , 72 , 156 , 81 , 47 , 59 , 85 , 227 , 192 , 159 , 216 , 211 , 243 , 141 , 177 , 255 , 167 , 62 , 220 , 134 , 119 , 215 , 166 , 17 , 251 , 244 , 186 , 146 , 145 , 100 , 131 , 241 , 51 , 239 , 218 , 44 , 181 , 178 , 43 , 136 , 209 , 153 , 203 , 140 , 132 , 29 , 20 , 129 , 151 , 113 , 202 , 95 , 163 , 139 , 87 , 60 , 130 , 196 , 82 , 92 , 28 , 232 , 160 , 4 , 180 , 133 , 74 , 246 , 19 , 84 , 182 , 223 , 12 , 26 , 142 , 222 , 224 , 57 , 252 , 32 , 155 , 36 , 78 , 169 , 152 , 158 , 171 , 242 , 96 , 208 , 108 , 234 , 250 , 199 , 217 , 256 , 212 , 31 , 110 , 67 , 188 , 236 , 83 , 137 , 254 , 122 , 93 , 73 , 201 , 50 , 194 , 249 , 154 , 248 , 109 , 22 , 219 , 89 , 150 , 68 , 233 , 205 , 230 , 70 , 66 , 143 , 10 , 193 , 204 , 185 , 101 , 176 , 210 , 198 , 172 , 30 , 65 , 98 , 41 , 46 , 14 , 116 , 80 , 2 , 90 , 195 , 37 , 123 , 138 , 42 , 91 , 240 , 6 , 13 , 71 , 111 , 112 , 157 , 126 , 16 , 206 , 18 , 39 , 213 , 76 , 79 , 214 , 121 , 48 , 104 , 54 , 117 , 125 , 228 , 237 , 128 , 106 , 144 , 55 , 162 , 94 , 118 , 170 , 197 , 127 , 61 , 175 , 165 , 229 , 25 , 97 , 253 , 77 , 124 , 183 , 11 , 238 , 173 , 75 , 34 , 245 , 231 , 115 , 35 , 33 , 200 , 5 , 225 , 102 , 221 , 179 , 88 , 105 , 99 , 86 , 15 , 161 , 49 , 149 , 23 , 7 , 58 , 40};

w64
shift_circ_left(w64 a, int b) {
	b = b % 64;
	if (b == 0)
		return a;
	return (a << b) | (a >> (64 - b));
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
	char *newkey;

	lenkey = strlen(key);
	newkey = malloc(16);

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

	//free(newkey);
	//free(L);
}

int main() {
	
	w64 *keys;
	char *ex = "Exemplo01";
	int i;

	generate_keys(ex, &keys);
	for (i = 1; i <= NSUBKEYS; i++) {
		printf("key[%d] = 0x%Lx\n", i, keys[i]);
	}

	return EXIT_SUCCESS;
}
