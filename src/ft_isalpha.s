default rel
global _ft_isalpha
section .text
_ft_isalpha:
        push    rbp                     ; Save callers base pointer value
        mov     rbp, rsp                ; Set up current function stack
        and     edi, -33                ; Convert lowercase value to uppercase
        add     edi, -65                ; Set set to alphabetical index
        xor     eax, eax                ; Clear out return register
        cmp     edi, 26
        setb    al                      ; Return 1 if edi < 26
        pop     rbp                     ; Revert to saved base pointer value
        ret                             ; Return to caller
