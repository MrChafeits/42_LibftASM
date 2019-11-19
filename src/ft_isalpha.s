default rel

global _ft_isalpha

section .text   align=16
; extern "C" int ft_isalpha(int c);
_ft_isalpha: ; PROC
        and     edi, -33                ; convert case
        add     edi, -65                ; set to alphabetical index
        xor     eax, eax                ; clear out return register
        cmp     edi, 26
        setb    al                      ; return 1 if edi < 26
        ret                             ; return to caller
; _ft_isalpha ENDP
