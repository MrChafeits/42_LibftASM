default rel

global _ft_atoi

section .text   align=16
; extern "C" int ft_atoi(const char *str);
_ft_atoi:
.L2:
        mov     al,  byte [rdi]
        lea     edx, [rax-9]
        cmp     dl,  4
        ja      .L13
.L3:
        inc     rdi
        jmp     .L2
.L13:
        cmp     al,  32
        je      .L3
        lea     edx, [rax-43]
        xor     ecx, ecx
        and     dl,  -3
        jne     .L4
        xor     ecx, ecx
        cmp     al,  45
        sete    cl
        inc     rdi
.L4:
        xor     eax, eax
.L5:
        movsx   edx, byte [rdi]
        sub     edx, 48
        cmp     edx, 9
        ja      .L14
        imul    rax, rax, 10
        movsx   rdx, edx
        inc     rdi
        add     rax, rdx
        jmp     .L5
.L14:
        lea     r8d, [rcx-1]
        test    rax, rax
        js      .L1
        mov     r8d, eax
        test    ecx, ecx
        je      .L1
        neg     r8d
.L1:
        mov     eax, r8d
        ret
; _ft_atoi ENDP
