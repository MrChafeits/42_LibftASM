default rel

global _ft_strcat

extern _ft_strlen
extern _ft_memcpy

section .text   align=16
; extern "C" char *ft_strcat(char *restrict s1, const char *restrict s2);
%define Rpar1   rdi
%define Rpar2   rsi
%define Rpar3   rdx

_ft_strcat: ; PROC
        push    Rpar1                   ; dest
        push    Rpar2                   ; src
        call    _ft_strlen              ; length of dest
        push    rax                     ; strlen(dest)
        mov     Rpar1, [rsp+8]          ; src
        call    _ft_strlen              ; length of src
        pop     Rpar1                   ; strlen(dest)
        pop     Rpar2                   ; src
        add     Rpar1, [rsp]            ; dest + strlen(dest)
        lea     Rpar3, [rax+1]          ; strlen(src)+1
        call    _ft_memcpy              ; copy
        pop     rax                     ; return dest
        ret
; _ft_strcat ENDP
