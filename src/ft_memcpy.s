default rel
global _ft_memcpy

section .text
; void *memcpy(void *restrict dst, const void *restrict src, size_t n)
_ft_memcpy:
        mov     rax, rdi
        mov     rcx, rdx
        rep     movsb                   ; Yeehaw, Linus
        ret
