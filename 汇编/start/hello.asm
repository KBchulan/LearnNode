.section .data

msg:
    .asciz "Hello, world!\n"

.section .text
.globl _start

_start:
    movq $1, %rax
    movq $1, %rdi
    movq $msg, %rsi
    movq $14, %rdx
    syscall

    movq $1, %rax

    movq $60, %rax
    xorq %rdi, %rdi
    syscall
