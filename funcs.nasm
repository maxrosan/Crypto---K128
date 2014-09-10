
BITS 64

section .bss
	key: resb 16
	keyex: resb 11*16

section .text
	global w128_xor_block

;void w128_xor_block (w8 a*, w8* b, w8* c)

w128_xor_block:

	movdqu xmm0, [rdi]
	movdqu xmm1, [rsi]
	pxor xmm0, xmm1
	movdqu [rdx], xmm0
	
	ret
