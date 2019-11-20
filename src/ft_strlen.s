default rel

global _ft_strlen

section .text   align=16
; extern "C" size_t ft_strlen(const char *s);
_ft_strlen:
        push    rbx                     ;
        push    rcx
        mov     rbx, rdi
        xor     al,  al
        mov     rcx, 0xffffffff         ; assuming maximum size of string is 4gb
        repne   scasb
        add     rbx, 1
        sub     rdi, rbx
        mov     rax, rdi
        pop     rcx
        pop     rbx
        ret
; _ft_strlen ENDP
