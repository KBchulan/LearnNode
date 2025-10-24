.section .data
    prompt:     .ascii "请输入一行字符(不超过80个):\n"
    prompt_len: .quad . - prompt
    letter:     .quad 0
    digit:      .quad 0
    other:      .quad 0
    buffer:     .space 81
    newline:    .ascii "\n"
    msg_letter: .ascii "字母数量: "
    msg_digit:  .ascii "数字数量: "
    msg_other:  .ascii "其他字符: "
    num_buf:    .space 20

.section .text
.globl _start
_start:
    movq $1, %rax
    movq $1, %rdi
    movq $prompt, %rsi
    movq prompt_len, %rdx
    syscall

    movq $0, %rax
    movq $0, %rdi
    movq $buffer, %rsi
    movq $80, %rdx
    syscall

    movq %rax, %r12
    
    movq $0, %r13
    
count_loop:
    cmpq %r12, %r13
    jge print_results
    
    movq $buffer, %rsi
    addq %r13, %rsi
    movb (%rsi), %al
    
    cmpb $10, %al
    je print_results
    
    cmpb $'A', %al
    jl not_letter
    cmpb $'z', %al
    jg not_letter
    cmpb $'Z', %al
    jle is_letter
    cmpb $'a', %al
    jge is_letter
    jmp not_letter

is_letter:
    incq letter
    jmp next_char

not_letter:
    cmpb $'0', %al
    jl not_digit
    cmpb $'9', %al
    jg not_digit
    incq digit
    jmp next_char

not_digit:
    incq other

next_char:
    incq %r13
    jmp count_loop

print_results:
    call print_letter_count
    call print_digit_count
    call print_other_count
    
    movq $60, %rax
    xorq %rdi, %rdi
    syscall

print_letter_count:
    pushq %rbp
    movq %rsp, %rbp
    
    movq $1, %rax
    movq $1, %rdi
    movq $msg_letter, %rsi
    movq $14, %rdx
    syscall
    
    movq letter, %rax
    call num_to_string
    
    movq $1, %rax
    movq $1, %rdi
    movq $newline, %rsi
    movq $1, %rdx
    syscall
    
    popq %rbp
    ret

print_digit_count:
    pushq %rbp
    movq %rsp, %rbp
    
    movq $1, %rax
    movq $1, %rdi
    movq $msg_digit, %rsi
    movq $14, %rdx
    syscall
    
    movq digit, %rax
    call num_to_string
    
    movq $1, %rax
    movq $1, %rdi
    movq $newline, %rsi
    movq $1, %rdx
    syscall
    
    popq %rbp
    ret

print_other_count:
    pushq %rbp
    movq %rsp, %rbp
    
    movq $1, %rax
    movq $1, %rdi
    movq $msg_other, %rsi
    movq $14, %rdx
    syscall
    
    movq other, %rax
    call num_to_string
    
    movq $1, %rax
    movq $1, %rdi
    movq $newline, %rsi
    movq $1, %rdx
    syscall
    
    popq %rbp
    ret

num_to_string:
    pushq %rbp
    movq %rsp, %rbp
    
    movq $num_buf, %rsi
    addq $19, %rsi
    movb $0, (%rsi)
    
    movq $10, %rcx
    
convert_loop:
    xorq %rdx, %rdx
    divq %rcx
    addb $'0', %dl
    decq %rsi
    movb %dl, (%rsi)
    testq %rax, %rax
    jnz convert_loop
    
    movq $1, %rax
    movq $1, %rdi
    movq $num_buf, %rdx
    addq $19, %rdx
    subq %rsi, %rdx
    syscall
    
    popq %rbp
    ret
