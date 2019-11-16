        extern _ft_strlen
        global _ft_puts

        section .text
_ft_puts:
        push    rbp                     ; Save callers base pointer value
        mov     rbp, rsp                ; Set up current function stack
        call    _ft_strlen
        mov     rdx, rax                ;
        mov     rsi, rdi                ;
        mov     rax, 2000004H           ; x86_64 osx write syscall
        mov     rdi, 1                  ;
        syscall
        mov     rdi, 1
        mov     rsi, 0AH
        mov     rdx, 1
        mov     rax, 2000004H
        syscall
        mov     rax, 0AH
        pop     rbp                     ; Revert to saved base pointer value
        ret                             ; Return to caller
