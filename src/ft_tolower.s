default rel
global _ft_tolower

section .text

_ft_tolower:
        push    rbp                     ; Save callers base pointer value
        mov     rbp, rsp                ; Set up current function stack
        lea     ecx, [rdi - 65]         ;
        mov     eax, edi                ;
        or      eax, 32                 ;
        cmp     ecx, 25                 ;
        cmova   eax, edi                ;
        pop     rbp                     ; Revert to saved base pointer value
        ret                             ; Return to caller
