default rel

global _ft_cat

extern _write
extern _read

section .text   align=16

; Ensure sane values for BUFSIZE
%ifndef BUFSIZE
; default buffer size if undefined
%define BUFSIZE 40H
%elif BUFSIZE <= 0 || BUFSIZE >= 8388608
; default buffer size if 0 >= BUFSIZE >= 8MiB
%define BUFSIZE 40H
%endif
; extern "C" void ft_cat(int fd);
_ft_cat: ; PROC
        push    rbx                     ; need to preserve rbx for caller
        mov     ebx, edi                ; fd
        sub     rsp, BUFSIZE            ; prepare buffer on stack
        jmp     L0OPS                   ; read loop
LO0PS:  mov     rsi, rsp                ; buf
        mov     edi, 1                  ; STDOUT_FILENO
        call    _write
L0OPS:  mov     edx, BUFSIZE            ; nbyte
        mov     rsi, rsp                ; buf
        mov     edi, ebx                ; filedes
        call    _read
        mov     rdx, rax                ; number of bytes actually read
        test    rax, rax
        jg      LO0PS                   ; continue if there's still more to read
        add     rsp, BUFSIZE            ; clean up stack
        pop     rbx                     ; revert registers
        ret
; _ft_cat ENDP
