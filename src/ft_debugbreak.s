global _ft_debugbreak

section .text
; extern "C" void ft_debugbreak(void);
_ft_debugbreak:
        int3
        nop
        ret
; _ft_debugbreak ENDP
