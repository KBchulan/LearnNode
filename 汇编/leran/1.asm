.section .data
    num1: .quad 5
    num2: .quad 10

.section  .text
    .globl _start

_start:
    movq num1(%rip), %rax
    movq num2(%rip), %rax

    movq $60, %rax
    xorq %rdi, %rdi
    syscall
