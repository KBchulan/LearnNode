.section .data
    number: .quad 7
    msg1: .string "Big\n"
    msg2: .string "Little\n"

.section .text
.global _start

_start:
    movq number(%rip), %rax
    cmpq $0, %rax
    jle negative

negative:
    movq $1, %rdi
    movq $msg2, %rsi
    movq $8, %rdx
    movq $1, %rax
    syscall
    jmp end

positive:
    movq $1, %rdi
    movq $msg1, %rsi
    movq $4, %rdx
    movq $1, %rax
    syscall
    jmp end

end:
    movq $60, %rax
    xorq %rdi, %rdi
    syscall
    