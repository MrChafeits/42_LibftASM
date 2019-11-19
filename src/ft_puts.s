default rel

global _ft_puts

extern _write

section .text   align=16
; extern "C" int ft_puts(const char *s);
_ft_puts: ; PROC
        push    rax
        or      rcx, -1
        xor     eax, eax
        mov     rsi, rdi
        repnz   scasb                   ; inline strlen
        mov     edi, 1                  ; add 1 to length
        not     rcx
        lea     rdx, [rcx-1]
        call    _write                  ; write s
        mov     edx, 1
        lea     rsi, [.nl]
        mov     edi, 1
        call    _write                  ; write '\n'
        pop     rdx
        ret
; _ft_puts ENDP

section .data
.nl:   db 10, 0
