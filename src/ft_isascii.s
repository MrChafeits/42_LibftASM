        section .text
        global _ft_isascii
_ft_isascii:
        push    rbp                     ; Save callers base pointer value
        mov     rbp, rsp                ; Set up current function stack
        xor     eax, eax
        cmp     edi, 128
        setb    al
        pop     rbp                     ; Revert to saved base pointer value
        ret                             ; Return to caller
