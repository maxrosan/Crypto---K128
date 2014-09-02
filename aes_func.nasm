BITS 64

section .bss
	key: resb 16
	keyex: resb 11*16

section .text
	global aesinit
	global aesencrypt
	global aesdecrypt

; void aesinit(char *key)
aesinit:

	movdqu xmm0, [rdi]
	movdqu [key], xmm0

	xorps xmm0, xmm0
	%assign i 0
	%rep 11
	movdqu [keyex + i], xmm0
	%assign i (i+16)
	%endrep

	movdqu xmm1, [key]
	movdqu [keyex], xmm1
	mov rcx, keyex+16

	aeskeygenassist xmm2, xmm1, 0x1
	call key_expansion_128
	aeskeygenassist xmm2, xmm1, 0x2
	call key_expansion_128
	aeskeygenassist xmm2, xmm1, 0x4
	call key_expansion_128
	aeskeygenassist xmm2, xmm1, 0x8
	call key_expansion_128
	aeskeygenassist xmm2, xmm1, 0x10
	call key_expansion_128
	aeskeygenassist xmm2, xmm1, 0x20
	call key_expansion_128
	aeskeygenassist xmm2, xmm1, 0x40
	call key_expansion_128
	aeskeygenassist xmm2, xmm1, 0x80
	call key_expansion_128
	aeskeygenassist xmm2, xmm1, 0x1B
	call key_expansion_128
	aeskeygenassist xmm2, xmm1, 0x36
	call key_expansion_128

	ret

aesdecrypt:

	movdqu xmm1, [rdi]
	movdqu xmm2, [keyex]
	movdqu xmm3, [keyex+0x10]
	movdqu xmm4, [keyex+0x20] ;
	movdqu xmm5, [keyex+0x30] ;
	movdqu xmm6, [keyex+0x40] ;
	movdqu xmm7, [keyex+0x50] ;
	movdqu xmm8, [keyex+0x60] ;
	movdqu xmm9, [keyex+0x70] ;
	movdqu xmm10,[keyex+0x80] ;
	movdqu xmm11,[keyex+0x90] ;
	movdqu xmm12,[keyex+0xA0] ;

	aesimc xmm3, xmm3
	aesimc xmm4, xmm4
	aesimc xmm5, xmm5
	aesimc xmm6, xmm6
	aesimc xmm7, xmm7
	aesimc xmm8, xmm8
	aesimc xmm9, xmm9
	aesimc xmm10, xmm10
	aesimc xmm11, xmm11

	pxor xmm1, xmm12 ; XORing
	aesdec xmm1, xmm11 ; Round 1
	aesdec xmm1, xmm10 ; Round 2
	aesdec xmm1, xmm9 ; Round 3
	aesdec xmm1, xmm8 ; Round 4
	aesdec xmm1, xmm7 ; Round 5
	aesdec xmm1, xmm6 ; Round 6
	aesdec xmm1, xmm5 ; Round 7
	aesdec xmm1, xmm4 ; Round 8
	aesdec xmm1, xmm3 ; Round 9
	aesdeclast xmm1, xmm2 ; Round 10 

	movdqu [rsi], xmm1

	ret

aesencrypt:

	movdqu xmm1, [rdi]
	movdqu xmm2, OWORD [keyex]
	movdqu xmm3, OWORD [keyex+0x10]
	movdqu xmm4, OWORD [keyex+0x20] ;
	movdqu xmm5, OWORD [keyex+0x30] ;
	movdqu xmm6, OWORD [keyex+0x40] ;
	movdqu xmm7, OWORD [keyex+0x50] ;
	movdqu xmm8, OWORD [keyex+0x60] ;
	movdqu xmm9, OWORD [keyex+0x70] ;
	movdqu xmm10, OWORD [keyex+0x80] ;
	movdqu xmm11, OWORD [keyex+0x90] ;
	movdqu xmm12, OWORD [keyex+0xA0] ; 

	pxor xmm1, xmm2 ; Round 0 (XORing)
	aesenc xmm1, xmm3 ; Round 1
	aesenc xmm1, xmm4 ; Round 2
	aesenc xmm1, xmm5 ; Round 3
	aesenc xmm1, xmm6 ; Round 4
	aesenc xmm1, xmm7 ; Round 5
	aesenc xmm1, xmm8 ; Round 6
	aesenc xmm1, xmm9 ; Round 7
	aesenc xmm1, xmm10 ; Round 8
	aesenc xmm1, xmm11 ; Round 9
	aesenclast xmm1, xmm12 ; Round 10 

	movdqu [rsi], xmm1
	
	ret

key_expansion_128:
	mov rdx, rcx
	pshufd xmm2, xmm2, 0b11111111
	pxor xmm2, xmm1
	movd eax, xmm2
	mov DWORD [rcx], eax
	add rcx, 4
	pshufd xmm1, xmm1, 011100101b
	movd ebx, xmm1
	xor eax, ebx
	mov DWORD [rcx], eax
	add rcx, 4
	pshufd xmm1, xmm1, 011100110b
	movd ebx, xmm1
	xor eax, ebx
	mov DWORD [rcx], eax
	add rcx, 4
	pshufd xmm1, xmm1, 011100111b
	movd ebx, xmm1
	xor eax, ebx
	mov DWORD [rcx], eax
	add rcx, 4
	movdqu xmm1, [rdx]
	ret
