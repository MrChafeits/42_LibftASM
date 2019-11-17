default rel

global  _A_memcpy

; Define prolog for this function
%macro PROLOGM  0
        mov     rcx, rdx                ; count
        mov     r9,  rdi                ; dest
%endmacro

; Define return from this function
%macro EPILOGM  0
        mov     rax, r9                 ; Return value = dest
        ret
%endmacro

section .text   align=16
_A_memcpy:       ; global label
A_memcpy@:      ; local label
        PROLOGM
        cmp     rcx, 40H
        jb      A1000

        ; count >= 64
        ; Calculate size of first block up to first regular boundary of dest
        mov     edx, edi
        neg     edx
        and     edx, 1FH
        jz      B3100                   ; Skip if dest aligned by 32

        ; edx = size of first partial block, 1 - 31 bytes
        test    dl, 3
        jz      B3030
        test    dl, 1
        jz      B3020
        ; move 1 byte
        movzx   eax, byte [rsi]
        mov     [rdi], al
        inc     rsi
        inc     rdi
B3020:  test    dl, 2
        jz      B3030
        ; move 2 bytes
        movzx   eax, word [rsi]
        mov     [rdi], ax
        add     rsi, 2
        add     rdi, 2
B3030:  test    dl, 4
        jz      B3040
        ; move 4 bytes
        mov     eax, [rsi]
        mov     [rdi], eax
        add     rsi, 4
        add     rdi, 4
B3040:  test    dl, 8
        jz      B3050
        ; move 8 bytes
        mov     rax, [rsi]
        mov     [rdi], rax
        add     rsi, 8
        add     rdi, 8
B3050:  test    dl, 16
        jz      B3060
        ; move 16 bytes
        movups  xmm0, [rsi]
        movups  [rdi], xmm0
        add     rsi, 16
        add     rdi, 16
B3060:  sub     rcx, rdx

B3100:  ; Now dest is aligned by 32. Any partial block has been moved

        ; Set up for loop moving 32 bytes per iteration:
        mov     rdx, rcx                ; Save count
        and     rcx, -20H               ; Round down to nearest multiple of 32
        add     rsi, rcx                ; Point to the end
        add     rdi, rcx                ; Point to the end
        sub     rdx, rcx                ; Remaining data after loop
        ; Check if count very big
        cmp     rcx, [CacheBypassLimit]
        ja      I3100                   ; Use non-temporal store if count > CacheBypassLimit
        neg     rcx                     ; Negative index from the end

H3100:  ; copy -rcx bytes in blocks of 32 bytes.

        ; Check for false memory dependence: The CPU may falsely assume
        ; a partial overlap between the written destination and the following
        ; read source if source is unaligned and
        ; (src-dest) modulo 4096 is close to 4096
        test    sil, 1FH
        jz      H3110                   ; aligned
        mov     eax, esi
        sub     eax, edi
        and     eax, 0FFFH              ; modulo 4096
        cmp     eax, 1000H - 200H
        ja      J3100

align 16
H3110:  ; main copy loop, 32 bytes at a time
        ; rcx has negative index from the end, counting up to zero
        vmovups ymm0, [rsi+rcx]
        vmovaps [rdi+rcx], ymm0
        add     rcx, 20H
        jnz     H3110
        vzeroupper                      ; end of AVX mode

H3120:  ; Move the remaining edx bytes (0 - 31):
        add     rsi, rdx
        add     rdi, rdx
        neg     rdx
        jz      H3500                   ; Skip if no more data
        ; move 16-8-4-2-1 bytes, aligned
        cmp     edx, -10H
        jg      H3200
        ; move 16 bytes
        movups  xmm0, [rsi+rdx]
        movups  [rdi+rdx], xmm0
        add     rdx, 10H
H3200:  cmp     edx, -8
        jg      H3210
        ; move 8 bytes
        movq    xmm0, qword [rsi+rdx]
        movq    qword [rdi+rdx], xmm0
        add     rdx, 8
        jz      H3500                   ; Early skip if count divisible by 8
H3210:  cmp     edx, -4
        jg      H3220
        ; move 4 bytes
        mov     eax, [rsi+rdx]
        mov     [rdi+rdx], eax
        add     rdx, 4
H3220:  cmp     edx, -2
        jg      H3230
        ; move 2 bytes
        movzx   eax, word [rsi+rdx]
        mov     [rdi+rdx], ax
        add     rdx, 2
H3230:  cmp     edx, -1
        jg      H3500
        ; move 1 byte
        movzx   eax, byte [rsi+rdx]
        mov     [rdi+rdx], al
H3500:  ; finished
        EPILOGM

I3100:  ; non-temporal move
        neg     rcx                     ; Negative index from the end

align 16
I3110:  ; main copy loop, 32 bytes at a time
        ; rcx has negative index from the end, counting up from zero
        vmovups ymm0, [rsi+rcx]
        vmovntps [rdi+rcx], ymm0
        add     rcx, 20H
        jnz     I3110
        sfence
        vzeroupper                      ; end of AVX mode
        jmp     H3120                   ; Move the remaining edx bytes (0 - 31)

align 16
J3100:  ; There is a false memory dependence.
        ; check if src and dest overlap, if not then it is safe
        ; to copy backwards to avoid false memory dependence
        mov     rax, rsi
        sub     rax, rdi
        neg     rcx                     ; size
        cmp     rax, rcx
        jnb     J3110                   ; Ok to copy backwards
        ; must copy forwards
        neg     rcx                     ; restore ecx
        jmp     H3110

J3110:  ; copy backwards, rcx = size. rsi, rdi = end of src, dest
        push    rsi
        push    rdi
        sub     rsi, rcx
        sub     rdi, rcx
J3120:  ; loop backwards
        vmovups ymm0, [rsi+rcx-20H]
        vmovups [rdi+rcx-20H], ymm0
        sub     rcx, 20H
        jnz     J3120
        vzeroupper
        pop     rdi
        pop     rsi
        jmp     H3120

align 16
        ; count < 64. Move 32-16-8-4-2-1 bytes
        ; multiple cup versions (SSSE3 and above)
A1000:  add     rsi, rcx                ; end of src
        add     rdi, rcx                ; end of dest
        neg     rcx                     ; negative index from the end
        cmp     ecx, -20H
        jg      A1100
        ; move 32 bytes
        ; movdqu is faster than 64-bit moves on processors with SSSE3
        movups  xmm0, [rsi+rcx]
        movups  xmm1, [rsi+rcx+10H]
        movups  [rdi+rcx], xmm0
        movups  [rdi+rcx+10H], xmm1
        add     rcx, 20H
A1100:  cmp     ecx, -10H
        jg      A1200
        ; move 16 bytes
        movups  xmm0, [rsi+rcx]
        movups  [rdi+rcx], xmm0
        add     rcx, 10H
A1200:  cmp     ecx, -8
        jg      A1300
        mov     rax, qword [rsi+rcx]
        mov     qword [rdi+rcx], rax
        add     rcx, 8
A1300:  cmp     ecx, -4
        jg      A1400
        ; move 4 bytes
        mov     eax, [rsi+rcx]
        mov     [rdi+rcx], eax
        add     rcx, 4
        jz      A1900                   ; early out if count divisible by 4
A1400:  cmp     ecx, -2
        jg      A1500
        ; move 2 bytes
        movzx   eax, word [rsi+rcx]
        mov     [rdi+rcx], ax
        add     rcx, 2
A1500:  cmp     ecx, -1
        jg      A1900
        ; move 1 byte
        movzx   eax, byte [rsi+rcx]
        mov     [rdi+rcx], al
A1900:  ; finished
        EPILOGM

section .data
align 16
CacheBypassLimit: DQ 40000H
