.section .data
    num1: .quad 10
    num2: .quad 20
    result: .quad 0

.section .text
.global _start

_start:
    # 加法运算
    movq num1(%rip), %rax
    addq num2(%rip), %rax
    movq %rax, result(%rip)

    movq num1(%rip), %rax
    imulq num2(%rip), %rax

    movq num1(%rip), %rax
    cqo
    movq num2(%rip), %rcx
    idivq %rcx

    movq $60, %rax
    xorq %rdi, %rdi
    syscall
    