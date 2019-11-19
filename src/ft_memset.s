default rel

global _ft_memset

section .text   align=16
; extern "C" void *ft_memset(void *b, int c, size_t len);
_ft_memset:
        mov     r9,  rdi                ; b
        mov     al,  sil                ; (unsigned char)c
        mov     rcx, rdx                ; len
        rep     stosb
        mov     rax, r9                 ; b
        ret                             ; return to caller
