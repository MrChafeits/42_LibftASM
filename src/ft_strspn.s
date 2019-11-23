default rel

global _ft_strspn

extern _A_strspn
; TODO: implement
section .text   align=16
; extern "C" size_t ft_strspn(const char *str, const char *sep);
_ft_strspn:
        jmp     _A_strspn
; _ft_strspn ENDP
