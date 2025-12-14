#if defined(_WIN64)
.text
.globl write

.extern GetStdHandle
.extern WriteConsoleA

write:
    subq $56, %rsp
    
    # Save parameter
    movq %rcx, %r15              # Save str
    movq %rdx, %r14              # Save len
    
    # GetStdHandle(STD_OUTPUT_HANDLE)
    movq $-11, %rcx
    call GetStdHandle
    
    # WriteConsoleA(handle, str, len, &written, NULL)
    movq %rax, %rcx              # handle
    movq %r15, %rdx              # str
    movl %r14d, %r8d             # len
    leaq 40(%rsp), %r9           # &written (di stack)
    movq $0, 32(%rsp)            # reserved = NULL
    
    call WriteConsoleA
    
    addq $56, %rsp
    ret
#else
    #error "Only Win64 supported"
#endif