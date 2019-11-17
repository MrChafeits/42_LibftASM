default rel

extern _write

global _ft_puts

section .text
_ft_puts:
        push    rax
        or      rcx, -1
        xor     eax, eax
        mov     rsi, rdi
        repnz   scasb
        mov     edi, 1
        not     rcx
        lea     rdx, [rcx-1]
        call    _write
        mov     edx, 1
        lea     rsi, [.nl]
        mov     edi, 1
        call    _write
        pop     rdx
        ret

section .data
.nl:   db 10, 0
