
SYNOPSIS
	./main -f algorithm -c -i input-file -o output-file -p password
	./main -f algorithm -d -i input-file -o output-file -p password

DESCRIPTION

	This program implements K128 algorithm in pure C and AES using a specific Intel instructions set named AES-NI. Iniatially the program was developed for a programming assigment of MAC5723-336 (Criptografia e Segurança de Dados), a cryptanalysis course at IME-USP. It uses Cipher-block chaining (CBC) as mode of operation and is able to encrypt and decrypt an entire file by using CBC.

OPTIONS

	-f algorithm
	Set what algorithm will be used to encrypt the blocks. There are two algorithms implemented: AES and K128. 
	  -f k128 (uses K128 algorithm)
	  -f aes  (uses AES algorithm)

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

INSTRUCTIONS TO COMPILE:

	Before trying to compile to program, it is necessary to have the following tools installed locally Makefile, GCC and NASM. AES-NI is supported by a specific family of Intel processors, just check if the target CPU supports AES-NI.

	cd <project directory>
	make

AUTHORS

	Written by Max Rosan <maxrosan@ime.usp.br>
