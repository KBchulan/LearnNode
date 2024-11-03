_section .data

.section .text
.globl _start

_start:
    movq $10, %rax
    movq $5, %rdx
    addq %rdx, %rax
    movq %rax, %rbx
    movq $1, %rax
    movq $0, %rdi