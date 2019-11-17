default rel
global _ft_isprint
section .text
_ft_isprint:
        push    rbp                     ; Save callers base pointer value
        mov     rbp, rsp                ; Set up current function stack
        add     edi, -32
        xor     eax, eax
        cmp     edi, 95
        setb    al
        pop     rbp                     ; Revert to saved base pointer value
        ret                             ; Return to caller
