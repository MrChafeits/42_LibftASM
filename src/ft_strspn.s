default rel

global _ft_strspn

section .text   align=8
; extern "C" size_t ft_strspn(const char *str, const char *sep);
_ft_strspn:
        mov     r8,  rsi
        mov     r9,  rdi

str_next10:
        mov     al,  [rdi]             ; read one byte from str
        test    al,  al
        jz      str_finished10         ; str finished
set_next10:
        mov     dl,  [rsi]
        test    dl,  dl
        jz      set_finished10
        inc     rsi
        cmp     al,  dl
        jne     set_next10
        ; character match found, goto next character
        inc     rdi
        mov     rsi, r8                ; set pointer
        jmp     str_next10

str_finished10: ; end of str, all match
set_finished10: ; end of set, mismatch found
        sub     rdi, r9                ; calculate position
        mov     rax, rdi
        ret
; _ft_strspn ENDP
