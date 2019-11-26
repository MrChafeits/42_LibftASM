default rel

global _ft_memcmp

section .text   align=16
; extern "C" int ft_memcmp(const void *p1, const void *p2, size_t n);
_ft_memcmp:
        xor     eax, eax
.L10:   cmp     rdx, rax
        je      .L20
        mov     cl,  byte [rdi+rax]
        inc     rax
        cmp     cl,  byte [rsi-1+rax]
        je      .L10
        sbb     eax, eax
        or      eax, 1
        ret
.L20:   xor     eax, eax
        ret
; _ft_memcmp ENDP
