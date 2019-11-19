default rel

global _ft_isalnum

section .text   align=16
; extern "C" int ft_isalnum(int c);
_ft_isalnum: ; PROC
        lea     eax, [rdi - 48]         ; c - '0'
        cmp     eax, 10
        setb    al                      ; isdigit
        and     edi, -33                ; convert case
        add     edi, -65                ; get alphabetical index
        cmp     edi, 26
        setb    cl                      ; isalpha
        or      cl, al                  ; combine results
        movzx   eax, cl                 ; move result into return register
        ret                             ; Return to caller
; _ft_isalnum ENDP
