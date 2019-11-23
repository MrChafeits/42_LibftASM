default rel

global _ft_strcspn

extern _A_strcspn
; TODO: implement

section .text   align=16
; extern "C" size_t ft_strcspn(const char *str, const char *sep);
_ft_strcspn:
        jmp     _A_strcspn
; _ft_strspn ENDP
