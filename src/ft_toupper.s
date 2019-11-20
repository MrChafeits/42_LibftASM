default rel

global _ft_toupper

section .text   align=16
; extern "C" int ft_toupper(int c);
_ft_toupper:
        lea     ecx, [rdi - 97]         ;
        mov     eax, edi                ;
        and     eax, 95                 ;
        cmp     ecx, 25                 ;
        cmova   eax, edi                ;
        ret                             ; Return to caller
; _ft_toupper ENDP;
