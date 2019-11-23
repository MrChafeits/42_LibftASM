default rel

global _ft_memcmp

extern _A_memcmp
; TODO: implement

section .text   align=16
; extern "C" int ft_memcmp(const void *p1, const void *p2, size_t n);
_ft_memcmp:
        jmp     _A_memcmp
