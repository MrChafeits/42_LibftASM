        section .text
        global _ft_strlen
_ft_strlen:
        push    rbp                     ; Save callers base pointer value
        mov     rbp, rsp                ; Set up current function stack
        xor     rax, rax                ;
        repne   scasb                   ;
        not     rcx                     ;
        add     rcx, 0xffffffffffffffff ;
        mov     rcx, rax                ;
        pop     rbp                     ; Revert to saved base pointer value
        ret                             ; Return to caller
