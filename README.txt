
SYNOPSIS
	./main -f algorithm -c -i input-file -o output-file -p password
	./main -f algorithm -d -i input-file -o output-file -p password
	./main -g rsa

DESCRIPTION

	This program implements K128 and RSA (128 bits) algorithms in pure C and AES using a specific Intel instructions set named AES-NI. Iniatially the program was developed for a programming assigment of MAC5723-336 (Criptografia e Segurança de Dados), a cryptanalysis course at IME-USP. Regarding the mode of operation for AES and K128, it uses Cipher-block chaining (CBC) and is able to encrypt and decrypt an entire file by using CBC. For RSA it uses Electronic Codebook (ECB). 

OPTIONS

	-f algorithm
	Set what algorithm will be used to encrypt the blocks. There are the following algorithms implemented: RSA, AES and K128. 
	  -f k128 (uses K128 algorithm)
	  -f aes  (uses AES algorithm)
	  -f rsa  (uses RSA algorithm)

	-c
	Encrypt the file and save the result in another file.

	-d
	Decrypt the file and save the result in another file.

	-i input-file
	Set the file whose data will be encrypted or decrypted

	-o output-file
	Set the file will be written with the data encrypted or decrypted

	-p password
	Defines the key will be used to encrypt or decrypt

	-g
	Generates public and private keys for RSA algorithm. The prime numbers are verified by using Fermat Test and Miller-Rabin probabilistic primality tests.

INSTRUCTIONS TO COMPILE:

	Before trying to compile to program, it is necessary to have the following tools installed locally: Makefile, GCC and NASM. AES-NI is supported by a specific family of Intel processors, just check if the target CPU supports AES-NI.

	cd <project directory>
	make

AUTHORS

	Written by Max Rosan <maxrosan@ime.usp.br>
