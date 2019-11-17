default rel
global _ft_strcat

extern _A_strlen
extern _A_memcpy

section .text   align=16
%define Rpar1   rdi
%define Rpar2   rsi
%define Rpar3   rdx

_ft_strcat:
        push    Rpar1                   ; dest
        push    Rpar2                   ; src
        call    _A_strlen               ; length of dest
        push    rax                     ; strlen(dest)
        mov     Rpar1, [rsp+8]          ; src
        call    _A_strlen               ; length of src
        pop     Rpar1                   ; strlen(dest)
        pop     Rpar2                   ; src
        add     Rpar1, [rsp]            ; dest + strlen(dest)
        lea     Rpar3, [rax+1]          ; strlen(src)+1
        call    _A_memcpy               ; copy
        pop     rax                     ; return dest
        ret
