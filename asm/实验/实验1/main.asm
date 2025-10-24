.section .data
    prompt1:     .ascii "Enter keyword: \0"
    prompt2:     .ascii "Enter Sentence: \0"
    match_msg:   .ascii "Match at location: \0"
    nomatch_msg: .ascii "No match!\0"
    newline:     .ascii "\n\0"
    
.section .bss
    .comm keyword, 100     # 关键字缓冲区
    .comm sentence, 1000   # 句子缓冲区
    .comm buffer, 20       # 数字转字符串缓冲区
    .comm CX, 8            # 关键字长度

.section .text
.globl _start

_start:
    movq $prompt1, %rsi
    call print_string
    movq $keyword, %rsi
    call read_string
    
    movq $keyword, %rdi
    call strlen
    movq %rax, CX
    
sentence_loop:
    movq $prompt2, %rsi
    call print_string
    movq $sentence, %rsi
    call read_string
    
    movq $keyword, %rsi
    movq $sentence, %rdi
    movq $0, %rbx
    
search_loop:
    movb (%rdi,%rbx), %al
    testb %al, %al
    jz not_found
    
    pushq %rdi
    pushq %rsi
    pushq %rbx
    
    leaq (%rdi,%rbx), %rdi
    movq CX, %rdx
    call strncmp
    
    popq %rbx
    popq %rsi
    popq %rdi
    
    testq %rax, %rax
    jz found
    
    incq %rbx
    jmp search_loop

found:
    movq $match_msg, %rsi
    call print_string
    movq %rbx, %rax
    incq %rax
    call print_number
    movq $newline, %rsi
    call print_string
    jmp sentence_loop

not_found:
    movq $nomatch_msg, %rsi
    call print_string
    movq $newline, %rsi
    call print_string
    jmp sentence_loop

strlen:
    xorq %rax, %rax
strlen_loop:
    cmpb $0, (%rdi,%rax)
    je strlen_done
    incq %rax
    jmp strlen_loop
strlen_done:
    ret

strncmp:
    xorq %rcx, %rcx
strncmp_loop:
    cmpq %rcx, %rdx
    je strncmp_equal
    movb (%rdi,%rcx), %al
    movb (%rsi,%rcx), %bl
    cmpb %bl, %al
    jne strncmp_notequal
    incq %rcx
    jmp strncmp_loop
strncmp_equal:
    xorq %rax, %rax
    ret
strncmp_notequal:
    movq $1, %rax
    ret

print_string:
    pushq %rsi
    movq %rsi, %rdi
    call strlen
    movq %rax, %rdx
    popq %rsi
    movq $1, %rax
    movq $1, %rdi
    syscall
    ret

read_string:
    movq $0, %rax
    movq $0, %rdi
    movq $1000, %rdx
    syscall
    
    movq %rsi, %rdi
    decq %rax
    movb $0, (%rdi,%rax)
    ret

print_number:
    movq $buffer+19, %rsi
    movb $0, (%rsi)
    movq $16, %rcx
print_number_loop:
    decq %rsi
    xorq %rdx, %rdx
    divq %rcx
    cmpb $9, %dl
    jg hex_letter
    addb $'0', %dl
    jmp store_digit
hex_letter:
    subb $10, %dl
    addb $'A', %dl
store_digit:
    movb %dl, (%rsi)
    testq %rax, %rax
    jnz print_number_loop
    
    decq %rsi
    movb $'x', (%rsi)
    decq %rsi
    movb $'0', (%rsi)
    
    call print_string
    ret
