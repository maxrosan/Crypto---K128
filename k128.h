
#ifndef K128H
#define K128H

#define w64 unsigned long long int
#define w32 unsigned int
#define w8 unsigned char


#define ROUNDS 12
#define NSUBKEYS (4*ROUNDS + 2)
#define DEBUG 0
#define BLOCKS_BYTE 16

#define LOG(MSG,...) if(DEBUG) printf(MSG "\n", ##__VA_ARGS__)
#define WFRT "%.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x"
#define WORD(buf) (buf)[0],(buf)[1],(buf)[2],(buf)[3],(buf)[4],(buf)[5],(buf)[6],(buf)[7]

typedef struct __CBC_Crypt {
	void (*crypt)   (struct __CBC_Crypt *, w8 in[BLOCKS_BYTE], w8 out[BLOCKS_BYTE]);
	void (*decrypt) (struct __CBC_Crypt *, w8 in[BLOCKS_BYTE], w8 out[BLOCKS_BYTE]);
} CBC_Crypt;


struct __K128_Crypt {
	CBC_Crypt cbc;
	w64  *__skeys;
};

void generate_keys(char *key, w64** k);
void tests_functions(void);
void k128_init(CBC_Crypt *c, char *mainkey);
void cbc_encode(CBC_Crypt *c, char *filename, char *fileoutput);
void cbc_decode(CBC_Crypt *c, char *filename, char *fileoutput);
#endif
