.section .data
    menu_prompt: .asciz "Choose function (1:Store, 2:Search, 3:Sort, 4:Display, 0:Exit): "
    prompt_name: .asciz "Input name: "
    prompt_phone: .asciz "Input telephone number: "
    not_found_msg: .asciz "Name not found!\n"
    display_header: .asciz "Name    Tel\n"
    newline: .asciz "\n"
    space: .asciz "    "

.section .bss
    .lcomm contacts_name, 1000
    .lcomm contacts_phone, 400
    .lcomm current_count, 8
    .lcomm name_buf, 20
    .lcomm phone_buf, 8
    .lcomm input_buf, 2

.section .text
    .globl _start

_start:
menu_loop:
    movq $menu_prompt, %rdi
    call print_string
    
    call input_char
    
    cmpb $'1', %al
    je store_contact
    cmpb $'2', %al
    je search_contact
    cmpb $'3', %al
    je sort_contacts
    cmpb $'4', %al
    je display_contacts
    cmpb $'0', %al
    je exit_program
    
    jmp menu_loop

store_contact:
    movq current_count, %rax
    cmpq $50, %rax
    jge menu_loop
    
    movq $prompt_name, %rdi
    call print_string
    
    movq $name_buf, %rdi
    movq $20, %rsi
    call input_string
    
    movq $prompt_phone, %rdi
    call print_string
    
    movq $phone_buf, %rdi
    movq $8, %rsi
    call input_string
    
    movq current_count, %rax
    movq $20, %rbx
    mulq %rbx
    leaq contacts_name(%rax), %rdi
    movq $name_buf, %rsi
    movq $20, %rcx
    call copy_string
    
    movq current_count, %rax
    movq $8, %rbx
    mulq %rbx
    leaq contacts_phone(%rax), %rdi
    movq $phone_buf, %rsi
    movq $8, %rcx
    call copy_string
    
    incq current_count
    
    jmp menu_loop

search_contact:
    movq $prompt_name, %rdi
    call print_string
    
    movq $name_buf, %rdi
    movq $20, %rsi
    call input_string
    
    movq $0, %r12
search_loop:
    cmpq current_count, %r12
    jge name_not_found
    
    movq %r12, %rax
    movq $20, %rbx
    mulq %rbx
    leaq contacts_name(%rax), %rsi
    movq $name_buf, %rdi
    call compare_string
    je found_name
    
    incq %r12
    jmp search_loop

found_name:
    movq %r12, %rax
    movq $8, %rbx
    mulq %rbx
    leaq contacts_phone(%rax), %rdi
    call print_string
    movq $newline, %rdi
    call print_string
    jmp menu_loop

name_not_found:
    movq $not_found_msg, %rdi
    call print_string
    jmp menu_loop

sort_contacts:
    movq current_count, %r13
    decq %r13
    jle menu_loop

outer_loop:
    movq $0, %r12
    movq $0, %r14

inner_loop:
    movq %r12, %rax
    movq $20, %rbx
    mulq %rbx
    leaq contacts_name(%rax), %rdi
    leaq contacts_name(%rax, %rbx), %rsi

    call compare_string
    jbe no_swap_needed

    movq $20, %rcx
    call swap_strings

    movq %r12, %rax
    movq $8, %rbx
    mulq %rbx
    leaq contacts_phone(%rax), %rdi
    leaq contacts_phone(%rax, %rbx), %rsi
    movq $8, %rcx
    call swap_strings

    movq $1, %r14

no_swap_needed:
    incq %r12
    cmpq %r13, %r12
    jl inner_loop

    testq %r14, %r14
    jz menu_loop

    decq %r13
    jg outer_loop

    jmp menu_loop

display_contacts:
    movq $display_header, %rdi
    call print_string

    movq $0, %r12
display_loop:
    cmpq current_count, %r12
    jge menu_loop

    movq %r12, %rax
    movq $20, %rbx
    mulq %rbx
    leaq contacts_name(%rax), %rdi
    call print_string

    movq $space, %rdi
    call print_string

    movq %r12, %rax
    movq $8, %rbx
    mulq %rbx
    leaq contacts_phone(%rax), %rdi
    call print_string
    
    movq $newline, %rdi
    call print_string

    incq %r12
    jmp display_loop

swap_strings:
    pushq %rcx
swap_loop:
    movb (%rdi), %al
    movb (%rsi), %bl
    movb %bl, (%rdi)
    movb %al, (%rsi)
    incq %rdi
    incq %rsi
    decq %rcx
    jnz swap_loop
    popq %rcx
    ret

copy_string:
    pushq %rcx
copy_loop:
    movb (%rsi), %al
    movb %al, (%rdi)
    incq %rsi
    incq %rdi
    decq %rcx
    jnz copy_loop
    popq %rcx
    ret

compare_string:
    pushq %rcx
    movq $20, %rcx
compare_loop:
    movb (%rdi), %al
    cmpb %al, (%rsi)
    jne compare_done
    incq %rdi
    incq %rsi
    decq %rcx
    jnz compare_loop
compare_done:
    popq %rcx
    ret

print_string:
    pushq %rbx
    movq %rdi, %rbx
    
    movq $0, %rdx
strlen_loop:
    movb (%rbx, %rdx), %al
    testb %al, %al
    jz strlen_done
    incq %rdx
    jmp strlen_loop
strlen_done:
    movq $1, %rax
    movq $1, %rdi
    movq %rbx, %rsi
    syscall
    
    popq %rbx
    ret

input_string:
    pushq %rbx
    movq %rdi, %rbx
    
    movq $0, %rax
    movq $0, %rdi
    movq %rbx, %rsi
    movq %rsi, %rdx
    syscall
    
    decq %rax
    movb $0, (%rbx, %rax)
    
    popq %rbx
    ret

input_char:
    movq $0, %rax
    movq $0, %rdi
    movq $input_buf, %rsi
    movq $1, %rdx
    syscall
    
    movb input_buf, %al
    
    pushq %rax
    movq $0, %rax
    movq $0, %rdi
    movq $input_buf, %rsi
    movq $1, %rdx
    syscall
    popq %rax
    
    ret

exit_program:
    movq $60, %rax
    xorq %rdi, %rdi
    syscall
