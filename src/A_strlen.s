default rel
global _A_strlen

section .text   align=16

%define Rscopy  rdi

_A_strlen:
        mov      rax,  rdi              ; get pointer to string from rdi
        mov      ecx,  edi              ; copy pointer (lower 32 bits)
        pxor     xmm0, xmm0             ; set to zero
        and      ecx,  0FH              ; lower 4 bits indicate misalignment
        and      rax,  -10H             ; align pointer by 16
        movdqa   xmm1, [rax]            ; read from nearest preceding boundary
        pcmpeqb  xmm1, xmm0             ; compare 16 bytes with zero
        pmovmskb edx,  xmm1             ; get one bit for each byte result
        shr      edx,  cl               ; shift out false bits
        shl      edx,  cl               ; shift back again
        bsf      edx,  edx              ; find first 1-bit
        jnz      L2                     ; found

        ; Main loop, search 16 bytes at a time
L1:     add      rax,  10H              ; increment pointer by 16
        movdqa   xmm1, [rax]            ; read 16 bytes aligned
        pcmpeqb  xmm1, xmm0             ; compare 16 bytes with zero
        pmovmskb edx,  xmm1             ; get one bit for each byte result
        bsf      edx,  edx              ; find first 1-bit
        ; (moving the bsf out of the loop and using test here would be faster on old processors,
        ;  but we are assuming most strings are short, and newer processors have higher priority)
        jz       L1

L2:     ; Zero-byte found. Compute string length
        sub      rax,  Rscopy           ; subtract start address
        add      rax,  rdx              ; add byte index
        ret
