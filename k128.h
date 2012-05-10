
#ifndef K128H
#define K128H

#define w64 unsigned long long int
#define w32 unsigned int
#define w8 unsigned char


#define ROUNDS 12
#define NSUBKEYS (4*ROUNDS + 2)
#define DEBUG 1
#define BLOCKS_BYTE 16

#define LOG(MSG,...) if(DEBUG) printf(MSG "\n", ##__VA_ARGS__)
#define WFRT "%.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x"
#define WORD(buf) (buf)[0],(buf)[1],(buf)[2],(buf)[3],(buf)[4],(buf)[5],(buf)[6],(buf)[7]


void generate_keys(char *key, w64** k);
void tests_functions(void);
void cbc_encode(char *filename, char *fileoutput);
void cbc_decode(char *filename, char *fileoutput);
#endif
