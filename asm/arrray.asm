.section .data
    array: .quad 1, 2, 3, 4, 5
    len: .quad 5
    sum: .quad 0

.section .text
.global _start

_start:
    xorq %rdi, %rdi
    xorq %rax, %rax

loop_start:
    cmpq len(%rip), %rdi
    jge loop_end

    leaq array(%rip), %rbx
    movq (%rbx,%rdi,8), %rbx
    addq %rbx, %rax

    incq %rdi
    jmp loop_start

loop_end:
    movq %rax, sum(%rip)

    movq $60, %rax
    xorq %rdi, %rdi
    syscall
