default rel

global _ft_strcspn

section .text   align=8
; extern "C" size_t ft_strcspn(const char *str, const char *sep);
_ft_strcspn:
        mov     r8,  rsi
        mov     r9,  rdi
str_next20:
        mov     al, [rdi]              ; read one byte from str
        test    al, al
        jz      str_finished20         ; str finished
set_next20:
        mov     dl, [rsi]
        test    dl, dl
        jz      set_finished20
        inc     rsi
        cmp     al, dl
        jne     set_next20
        ; character match found, stop search
        jmp     str_finished20

set_finished20: ; end of set, mismatch found
        inc     rdi
        mov     rsi, r8                ; set pointer
        jmp     str_next20

str_finished20: ; end of str, all match
        sub     rdi, r9                ; calculate position
        mov     rax, rdi
        ret
; _ft_strspn ENDP
