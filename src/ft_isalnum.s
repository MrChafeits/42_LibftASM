default rel
global _ft_isalnum

section .text

_ft_isalnum:
        push    rbp                     ; Save callers base pointer value
        mov     rbp, rsp                ; Set up current function stack
        lea     eax, [rdi - 48]         ;
        cmp     eax, 10
        setb    al                      ; isdigit
        and     edi, -33
        add     edi, -65
        cmp     edi, 26
        setb    cl                      ; isalpha
        or      cl, al                  ; combine results
        movzx   eax, cl                 ; move result into return register
        pop     rbp                     ; Revert to saved base pointer value
        ret                             ; Return to caller
