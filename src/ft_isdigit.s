default rel

global _ft_isdigit

section .text   align=16
; extern "C" int ft_isdigit(int c);
_ft_isdigit: ; PROC
        add     edi, -48                ; c - '0'
        xor     eax, eax                ; clear out return register
        cmp     edi, 10
        setb    al                      ; return 1 if 0 <= edi < 10
        ret                             ; return to caller
; _ft_isdigit ENDP
