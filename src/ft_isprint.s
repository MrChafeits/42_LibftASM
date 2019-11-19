default rel

global _ft_isprint

section .text   align=16
; extern "C" int ft_isprint(int c);
_ft_isprint: ; PROC
        add     edi, -32                ; c - ' '
        xor     eax, eax                ; clear return value
        cmp     edi, 95
        setb    al                      ; return 1 if 0 <= (c - ' ') < 95
        ret                             ; return to caller
; _ft_isprint ENDP
