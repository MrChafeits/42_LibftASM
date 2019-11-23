default rel

global _ft_rdtsc

extern _ReadTSC
; TODO: implement

section .text   align=16
; extern "C" int ft_rdtsc(void);
_ft_rdtsc:
        jmp     _ReadTSC
