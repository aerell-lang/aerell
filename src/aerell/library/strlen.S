#if defined(_WIN64)
.text
.globl strlen

strlen:
    xorq %rax, %rax

.Lloop:
    cmpb $0, (%rcx, %rax)
    je .Ldone
    incq %rax
    jmp .Lloop

.Ldone:
    ret
#else
    #error "Only Win64 supported"
#endif