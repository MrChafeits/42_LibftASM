default rel
global _ft_strdup

extern malloc
extern A_memcpy
extern _ft_strlen

section .text   align=16

_ft_strdup:
%if 1
        push    rbp
        mov     rbp, rdi
        push    rbx
        push    rcx

        call    _ft_strlen

        lea     rdi, [rax+1]

        mov     rbx, rax

        call    malloc

        test    rax, rax
        je      RETNOW

        mov     rdi, rax
        mov     rsi, rbp
        mov     rcx, rbx
        rep     movsb

RETNOW: pop     rdx
        pop     rbx
        pop     rbp
        ret
%else
        push    r15
        push    r14
        push    rbx
        mov     r14, rdi

        call    _ft_strlen
        mov     r15, rax

        lea     rdi, [rax+1]
        call    malloc
        mov     rbx, rax

        test    rax, rax
        je      LBBOI
        test    r15, r15
        je      LBBOI

        mov     rdi, rbx
        mov     rsi, r14
        mov     rdx, r15
        call    A_memcpy

LBBOI:  mov     rax, rbx
        pop     rbx
        pop     r14
        pop     r15
        ret
%endif
; -----------------------------------------------------------------------------
        ; push    rbp                     ; Save callers base pointer value
        ; mov     rbp, rsp                ; Set up current function stack
        ; call    _ft_strlen
        ; lea     rdi,

        ; pop     rbp                     ; Revert to saved base pointer value
        ; ret                             ; Return to caller
