default rel

global _ft_strdup

extern _malloc
extern _ft_strlen

section .text   align=16
; extern "C" char *ft_strdup(const char *s1);
_ft_strdup: ; PROC
        push    rbp
        mov     rbp, rdi
        push    rbx
        push    rcx
        call    _ft_strlen
        lea     rdi, [rax+1]
        mov     rbx, rax
        call    _malloc
        test    rax, rax
        je      ERROR
        mov     rdi, rax
        mov     rsi, rbp
        lea     rcx, [rbx+1]
        rep     movsb
ERROR:  pop     rdx
        pop     rbx
        pop     rbp
        ret
; _ft_strdup ENDP
