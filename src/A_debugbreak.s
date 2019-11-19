;****************************** A_debugbreak.s *********************************
; Description:
; Makes a debug breakpoint. Works only when running under a debugger.
;
;*******************************************************************************
;
; C++ prototype:
; extern "C" void A_DebugBreak(void);

global _A_DebugBreak

section .text

_A_DebugBreak:
        int3
        nop
        ret
; _A_DebugBreak ENDP
