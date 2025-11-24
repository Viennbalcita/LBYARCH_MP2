; ARMAS, AARON MARK CRISTOBAL & BALCITA, VIENN ROWEN GATCHALIAN S18B
; imgCvtGrayInttoFloat - Converts int grayscale pixels to float (0.0 to 1.0)
; Formula: float_value = int_value / 255.0
; Windows x64: RCX = int* input, RDX = float* output, R8D = pixel count

section .data
    one_over_255: dd 0.00392156862745098    ; 1/255 constant

section .text
bits 64
default rel
global imgCvtGrayInttoFloat

imgCvtGrayInttoFloat:
    push rbp
    mov rbp, rsp
    push rbx
    
    mov r9, rcx             ; r9 = input pointer
    mov r10, rdx            ; r10 = output pointer
    mov ecx, r8d            ; ecx = loop counter
    
    test ecx, ecx
    jz .done
    
.loop:
    mov eax, DWORD [r9]
    cvtsi2ss xmm0, eax      ; Convert int to float (scalar SIMD)
    movss xmm1, DWORD [rel one_over_255]
    mulss xmm0, xmm1        ; Multiply by 1/255 (scalar SIMD)
    movss DWORD [r10], xmm0
    
    add r9, 4
    add r10, 4
    dec ecx
    jnz .loop
    
.done:
    pop rbx
    pop rbp
    ret
