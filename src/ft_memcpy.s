default rel

global _ft_memcpy

section .text   align=16
; void *memcpy(void *restrict dst, const void *restrict src, size_t n)
_ft_memcpy:
        mov     rax, rdi                ; dst
        mov     rcx, rdx                ; n
        rep     movsb                   ; Yeehaw, Linus
        ret                             ; return to caller
