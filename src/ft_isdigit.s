        section .text
        global _ft_isdigit
_ft_isdigit:
        push    rbp                     ; Save callers base pointer value
        mov     rbp, rsp                ; Set up current function stack
        add     edi, -48                ; Subtract '0' from argument
        xor     eax, eax                ; Clear out return register
        cmp     edi, 10
        setb    al                      ; Return 1 if edi < 10
        pop     rbp                     ; Revert to saved base pointer value
        ret                             ; Return to caller
