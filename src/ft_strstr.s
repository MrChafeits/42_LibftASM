default rel

global _ft_strstr

section .text   align=16
; extern "C" char *ft_strstr(const char *s1, const char *s2);
_ft_strstr:
        mov     rax, rdi
        mov     dil, byte [rsi]
        test    dil, dil
        je      .L50
        mov     dl,  byte [rax]
.L10:
        test    dl,  dl
        je      .L40
        cmp     dl,  dil
        jne     .L30
        xor     edx, edx
.L20:
        mov     cl,  byte [rsi+rdx]
        test    cl,  cl
        je      .L50
        cmp     byte [rax+rdx], cl
        lea     rdx, [rdx+1]
        je      .L20
.L30:
        mov     dl, byte [rax+1]
        inc     rax
        jmp     .L10
.L40:
        xor     eax, eax
.L50:
        ret
; _ft_strstr ENDP
