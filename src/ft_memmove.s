default rel

global _ft_memmove

section .text   align=16
; extern "C" void *ft_memmove(void *dst, const void *src, size_t n);
_ft_memmove:
        mov     rax, rdi                        ; dst
        cmp     rdi, rsi
        jnb     .L3                             ; if (!(src < dst))
        xor     ecx, ecx
.L2: ; fill in backwards from the ends
        cmp     rdx, rcx
        je      .L1
        mov     dil, byte [rsi+rcx]
        mov     byte [rax+rcx], dil
        inc     rcx
        jmp     .L2
.L1: ; done
        ret
.L3: ; fill in forwards from the beginnings
        dec     rdx
        cmp     rdx, -1
        je      .L4
        mov     cl,  byte [rsi+rdx]
        mov     byte [rax+rdx], cl
        jmp     .L3
.L4: ; done
        ret
; _ft_memmove ENDP
