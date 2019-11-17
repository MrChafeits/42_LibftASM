default rel

global _read_tsc

section .text align=16

; This function returns the value of the time stamp counter, which counts
; clock cycles. To cont how many clock cycles a piece of code takes, call
; Rdtsc before and after the code to measure and calculate the difference.

; The number of clock cycles taken by the read_tsc function itself is approximately:
; Core 2:                       730
; Pentium 4:                    700
; Pentium II and Pentium III:   225
; AMD Athlon 64, Opteron:       126
; Does not work on 80386 and 80486

; Note that clock counts may not be fully repoducible on Intel Core and
; Core 2 processors because the clock frequency can change.

_read_tsc:
        push    rbx                     ; ebx is modified by cpuid
        sub     eax, eax                ; 0
        cpuid                           ; serialize
        rdtsc                           ; read time stamp counter into edx:eax
        shl     rdx, 32
        or      rax, rdx                ; combine into 64 bit register
        push    rax
        sub     eax, eax
        cpuid                           ; serialize
        pop     rax                     ; return value
        pop     rbx
        ret
