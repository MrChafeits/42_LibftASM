default rel
global _ft_strlen

section .text

_ft_strlen:
        push    rbx
        push    rcx
        mov     rbx, rdi
        xor     al,  al
        mov     rcx, 0xffffffff
        repne   scasb
        add     rbx, 1
        sub     rdi, rbx
        mov     rax, rdi
        pop     rcx
        pop     rbx
        ret
