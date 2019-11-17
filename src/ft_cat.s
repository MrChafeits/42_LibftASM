default rel
global _ft_cat

extern _write
extern _read

section .text

_ft_cat:
        push    rbx
        mov     ebx, edi
        sub     rsp, 40H

        jmp     L0OPS
LO0PS:  mov     rsi, rsp
        mov     edi, 1
        call    _write

L0OPS:  mov     edx, 40H
        mov     rsi, rsp
        mov     edi, ebx
        call    _read
        mov     rdx, rax
        test    rax, rax
        jg      LO0PS
        add     rsp, 40H

        pop     rbx
        ret
; -----------------------------------------------------------------------------
        push    rbp                     ; Save callers base pointer value
        mov     rbp, rsp                ; Set up current function stack

        pop     rbp                     ; Revert to saved base pointer value
        ret                             ; Return to caller
