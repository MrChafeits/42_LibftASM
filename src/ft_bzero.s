default rel
section .text   align=16
global _ft_bzero

; void bzero(void *s, size_t n)
%define Rdest   rdi
%define Rcount  rdx
%define Rdest2  rcx
%define Rcount2 rsi
_ft_bzero:
        mov     eax, 0
        imul    eax, 0x01010101         ; Broadcast c into all bytes of eax
        mov     Rdest2, Rdest           ; save dest
B010:   cmp     Rcount, 16
        ja      B100
B050:   lea     r10, [MemsetJTab]       ; SSE2 version comes in here
        jmp     qword [r10+Rcount*8]    ; jump table for small counts

; Separate code for each count from 0 to 16:
M16:    mov     [Rdest+12], eax
M12:    mov     [Rdest+8],  eax
M08:    mov     [Rdest+4],  eax
M04:    mov     [Rdest],    eax
M00:    mov     rax, Rdest2             ; return dest
        ret

M15:    mov     [Rdest+11], eax
M11:    mov     [Rdest+7],  eax
M07:    mov     [Rdest+3],  eax
M03:    mov     [Rdest+1],  ax
M01:    mov     [Rdest],    al
        mov     rax, Rdest2             ; return dest
        ret

M14:    mov     [Rdest+10], eax
M10:    mov     [Rdest+6],  eax
M06:    mov     [Rdest+2],  eax
M02:    mov     [Rdest],    ax
        mov     rax, Rdest2
        ret

M13:    mov     [Rdest+9],  eax
M09:    mov     [Rdest+5],  eax
M05:    mov     [Rdest+1],  eax
        mov     [Rdest],    al
        mov     rax, Rdest2
        ret

B100:   ; AVX version, Rcount > 16
        movd    xmm0, eax
        pshufd  xmm0, xmm0, 0           ; Broadcast c into all bytes of xmm0

        lea     rax, [Rdest+Rcount]     ; point to end

        cmp     Rcount, 0x20
        jbe     K600                    ; faster to use xmm registers if small

        ; Store the first possibly unaligned 16 bytes
        ; It is faster to always write 16 bytes, possibly overlapping
        ; with the subsequent regular part, than to make possible mispredicted
        ; branches depending on the size of the first part.
        movups  oword [Rdest], xmm0

        ; store another 16 bytes, aligned
        add     Rdest, 0x10
        and     Rdest, -0x10
        movaps  oword [Rdest], xmm0

        ; go to next 32 bytes boundary
        add     Rdest, 0x10
        and     Rdest, -0x20

        ; Check if count is very big
        cmp     Rcount, [MemsetCacheLimit]
        ja      K300                    ; Use non-temporal store if count > MemsetCacheLimit

        ; find last 32 bytes boundary
        mov     Rcount, rax
        and     Rcount, -0x20

        ; - size of 32-bytes blocks
        sub     Rdest, Rcount
        jnb     K200                    ; Jump if not negative

        ; extend value to 256 bits
        vinsertf128 ymm0,ymm0,xmm0,1

align   16
K100:   ; Loop through 32-bytes blocks. Register use is swapped
        ; Rcount = end of 32-byes blocks part
        ; Rdest = negative index from the end, counting up to zero
        vmovaps [Rcount+Rdest], ymm0
        add     Rdest, 0x20
        jnz     K100
        vzeroupper

K200:   ; the last part from Rcount to rax is < 32 bytes. write last 32 bytes with overlap
        movups  [rax-0x20], xmm0
        movups  [rax-0x10], xmm0
        mov     rax, Rdest2             ; return dest
        ret

K300:   ; Use non-temporal moves, same code as above:

        ; find last 32 bytes boundary
        mov     Rcount, rax
        and     Rcount, -0x20

        ; - size of 32-bytes blocks
        sub     Rdest, Rcount
        jnb     K500                    ; Jump if not negative

        ; extend value to 256 bits
        vinsertf128 ymm0,ymm0,xmm0,1

align   16
K400:   ; Loop through 32-bytes blocks. Register use is swapped
        ; Rcount end of 32-bytes blocks part
        ; Rdest = negative index from the end, counting up from zero
        vmovntps [Rcount+Rdest], ymm0
        add     Rdest, 0x20
        jnz     K400
        sfence
        vzeroupper

K500:   ; the last part from Rcount to rax is < 32 bytes. write last 32 bytes with overlap
        movups  [rax-0x20], xmm0
        movups  [rax-0x10], xmm0
        mov     rax, Rdest2             ; return dest
        ret

K600:   ; 16 < count <= 32
        movups  [Rdest], xmm0
        movups  [rax-0x10], xmm0
        mov     rax, Rdest2             ; return dest
        ret
;-------------------------------------------------------------------------------
        push    rbp                     ; Save callers base pointer value
        mov     rbp, rsp                ; Set up current function stack
        mov     r9,  rdi
        mov     al,  0
        mov     rcx, rsi
        rep     stosb
        mov     rax, r9
        pop     rbp                     ; Revert to saved base pointer value
        ret                             ; Return to caller
SECTION .data
align 16
; Jump table for count from 0 to 16:
MemsetJTab:DQ M00, M01, M02, M03, M04, M05, M06, M07
           DQ M08, M09, M10, M11, M12, M13, M14, M15, M16

; Bypass
MemsetCacheLimit: DQ 0
