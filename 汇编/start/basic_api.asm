.section .data
hello:
    .asciz "Hello, World!"

.section .text
.globl _start

_start:
    movl $1, %eax
    movl $1, %edi
    lea hello, %rsi
    movl $13, %edx
    syscall

    movl $60, %eax
    xor %edi, %edi
    syscall
