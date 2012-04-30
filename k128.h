
#ifndef K128H
#define K128H

#define w64 unsigned long long int
#define w32 unsigned int
#define w8 unsigned char


#define ROUNDS 12
#define NSUBKEYS (4*ROUNDS + 2)
#define DEBUG 1	


void generate_keys(char *key, w64** k);
void tests_functions(void);
void cbc_encode(char *filename, char *fileoutput);
void cbc_decode(char *filename, char *fileoutput);
#endif
