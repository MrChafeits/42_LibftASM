default rel
global _ft_memcpy

section .text
; void *memcpy(void *restrict dst, const void *restrict src, size_t n)
_ft_memcpy:
        push    rbp                     ; Save callers base pointer value
        mov     rbp, rsp                ; Set up current function stack
        mov     rax, rdi
        mov     rcx, rdx
        rep     movsb                   ; Yeehaw, Linus
        pop     rbp                     ; Revert to saved base pointer value
        ret                             ; Return to caller
