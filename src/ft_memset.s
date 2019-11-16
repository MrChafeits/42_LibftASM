        section .text
        global _ft_memset
_ft_memset:
        push    rbp                     ; Save callers base pointer value
        mov     rbp, rsp                ; Set up current function stack

        mov     r9,  rdi
        mov     al,  sil
        mov     rcx, rdx
        rep     stosb
        mov     rax, r9

        pop     rbp                     ; Revert to saved base pointer value
        ret                             ; Return to caller
