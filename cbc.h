
#ifndef CBCH
#define CBCH

#include <stdint.h>

#define w64 uint64_t
#define w32 unsigned int
#define w8 unsigned char

#define BLOCKS_BYTE 16
#define ROUNDS 12
#define NSUBKEYS (4*ROUNDS + 2)
#define DEBUG 0
#define ENABLE_TESTS 0

#define LOG(MSG,...) if(DEBUG) printf(MSG "\n", ##__VA_ARGS__)
#define WFRT "%.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x"
#define WORD(buf) (buf)[0],(buf)[1],(buf)[2],(buf)[3],(buf)[4],(buf)[5],(buf)[6],(buf)[7]

typedef struct __CBC_Crypt {
	void (*crypt)   (struct __CBC_Crypt *, w8* in, w8* out);
	void (*decrypt) (struct __CBC_Crypt *, w8* in, w8* out);
	void (*encode)(struct __CBC_Crypt *c, char *filename, char *fileoutput);
	void (*decode)(struct __CBC_Crypt *c, char *filename, char *fileoutput);
	int word_length;
} CBC_Crypt;

void uint64_to_bytes(w64 in, int n, w8* out);

void cbc_init(CBC_Crypt *c);
void cbc_encode(CBC_Crypt *c, char *filename, char *fileoutput);
void cbc_decode(CBC_Crypt *c, char *filename, char *fileoutput);

void ebc_init(CBC_Crypt *c);
void ebc_encode(CBC_Crypt *c, char *filename, char *fileoutput);
void ebc_decode(CBC_Crypt *c, char *filename, char *fileoutput);

#endif
