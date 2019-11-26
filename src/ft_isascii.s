default rel

global _ft_isascii

section .text   align=16
; extern "C" int ft_isascii(int c);
_ft_isascii: ; PROC
        xor     eax, eax                ; clear return value
        cmp     edi, 128                ; check
        setb    al                      ; return 1 if 0 <= c <= 127
        ret
; _ft_isascii ENDP
