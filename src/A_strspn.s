default rel

global _A_strspn
global _A_strcspn

section .text

align 16

_A_strspn:
	mov	r8,  rsi
	xor	ecx, ecx		; span counter
str_next:
	movdqu	xmm2, [rdi]		; str
	movdqu	xmm1, [rsi]		; set (the memory read port is likely to be vacant early, so no need to put this read outside the loop)
	pcmpistrm xmm1, xmm2, 00000000b	; find in set, return bit mask in xmm0
	movd	eax, xmm0
	jns	set_extends
set_finished:
	cmp	ax,  -1
	jne	str_finished
	; first 16 characters matched, continue with next 16 characters (a terminating zero would never match)
	add	rdi, 16			; next 16 bytes of str
	add	rcx, 16			; count span
	jmp	str_next

str_finished:
	not	eax
	bsf	eax, eax
	add	rax, rcx
	ret

set_loop:
	or	eax, set_extends	; accumulate matches
set_extends: ; the set is more than 16 bytes
	add	rsi, 16
	movdqu	xmm1, [rsi]		; next part of set
	pcmpistrm xmm1, xmm2, 00000000b	; find in set, return bit mask in xmm0
	movd	edx, xmm0
	jns	set_loop
	mov	rsi, r8			; restore set pointer
	or	eax, edx		; accumulate matches
	jmp	set_finished

align 16
_A_strcspn:
	mov	r8,  rsi
	xor	ecx, ecx		; span counter
str_next2:
	movdqu	xmm2, [rdi]		; str
	movdqu	xmm1, [rsi]		; set
	pcmpistrm xmm1, xmm2, 00110000b ; find in set, invert valid bits, return bit mask in xmm0
	movd	eax, xmm0
	jns	set_extends2
set_finished2:
	cmp	ax, -1
	jne	str_finished2
	; first 16 characters matched, continue with next 16 characters (a terminating zero would never match)
	add	rdi, 16			; next 16 bytes of str
	add	rcx, 16			; count span
	jmp	str_next2

str_finished2:
	not	eax
	bsf	eax, eax
	add	rax, rcx
	ret

set_loop2:
	and	eax, edx		; accumulate matches
set_extends2: ; the set is more than 16 bytes
	add	rsi, 16
	movdqu	xmm1, [rsi]		; next part of set
	pcmpistrm xmm1, xmm2, 00110000b ; find in set, invert valid bits, return bit mask in xmm0
	movd	edx, xmm0
	jns	set_loop2
	mov	rsi, r8			; restore set pointer
	and	eax, edx		; accumulate matches
	jmp	set_finished2
