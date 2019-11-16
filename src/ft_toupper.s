        section .text
        global _ft_toupper
_ft_toupper:
        push    rbp                     ; Save callers base pointer value
        mov     rbp, rsp                ; Set up current function stack
        lea     ecx, [rdi - 97]         ;
        mov     eax, edi                ;
        and     eax, 95                 ;
        cmp     ecx, 25                 ;
        cmova   eax, edi                ;
        pop     rbp                     ; Revert to saved base pointer value
        ret                             ; Return to caller
