default rel

global _ft_tolower

section .text   align=16
; extern "C" int ft_tolower(int c);
_ft_tolower:
        lea     ecx, [rdi - 65]         ;
        mov     eax, edi                ;
        or      eax, 32                 ;
        cmp     ecx, 25                 ;
        cmova   eax, edi                ;
        ret
; _ft_tolower ENDP
