#if defined(_WIN64)
.text
.globl print

.extern strlen
.extern write

print:
    call strlen
    movq %rax, %rdx
    call write
    ret
#else
    #error "Only Win64 supported"
#endif