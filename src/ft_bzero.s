default rel

global _ft_bzero

section .text   align=16
; extern "C" void ft_bzero(void *s, size_t n)
_ft_bzero: ; PROC
        xor     eax, eax                        ; clear out byte to store
        mov     rcx, rsi                        ; n
        rep     stosb
        ret
; _ft_bzero ENDP
