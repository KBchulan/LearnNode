.section .data
    prompt:     .ascii "Please enter the n value: "
    prompt_len: .quad . - prompt
    newline:    .ascii "\n"
    buffer:     .space 256
    array:      .zero 1600

.section .text
    .globl _start

_start:
    pushq %rbp
    movq %rsp, %rbp

main_loop:
    # 打印提示信息
    movq $1, %rax
    movq $1, %rdi           
    movq $prompt, %rsi
    movq prompt_len, %rdx
    syscall

    # 读取输入
    movq $0, %rax
    movq $0, %rdi       
    movq $buffer, %rsi   
    movq $256, %rdx
    syscall

    # 将字符串转换为数字
    movq $buffer, %rsi
    call atoi
    movq %rax, %rdi

    # 检查输入值是否有效 (1-100)
    cmpq $1, %rdi
    jl main_loop
    cmpq $100, %rdi
    jg main_loop

    # 调用斐波那契函数
    call fibonacci

    # 将结果转换为字符串并打印
    pushq %rdx              # 保存高64位
    movq %rax, %rdi
    movq $buffer, %rsi
    call ltoa              # 返回值在rax中是字符串长度
    movq %rax, %rdx        # 将长度移到rdx用于打印
    popq %r8               # 恢复之前保存的高64位（但现在不使用）

    # 打印结果
    movq $1, %rax           # sys_write
    movq $1, %rdi           # stdout
    movq $buffer, %rsi
    # rdx已经包含了字符串长度
    syscall

    # 打印换行
    movq $1, %rax
    movq $1, %rdi
    movq $newline, %rsi
    movq $1, %rdx
    syscall

    jmp main_loop

# 字符串转整数(入参：%rsi)
atoi:
    pushq %rbp
    movq %rsp, %rbp
    xorq %rax, %rax
    xorq %rcx, %rcx
atoi_loop:
    movb (%rsi), %cl
    cmpb $10, %cl
    je atoi_done
    cmpb $'0', %cl
    jl atoi_done
    cmpb $'9', %cl
    jg atoi_done
    subb $'0', %cl
    imulq $10, %rax
    addq %rcx, %rax
    incq %rsi
    jmp atoi_loop
atoi_done:
    movq %rbp, %rsp
    popq %rbp
    ret

# 整数转字符串
ltoa:
    pushq %rbp
    movq %rsp, %rbp
    pushq %rbx
    pushq %r12
    
    xorq %rcx, %rcx
    
    # 特殊处理0
    testq %rdi, %rdi
    jnz ltoa_process
    movb $'0', (%rsi)
    movb $0, 1(%rsi)
    movq $1, %rax
    jmp ltoa_exit

ltoa_process:
    movq %rsi, %r12
    movq %rdi, %rax
    movq $10, %rbx

convert_loop:
    xorq %rdx, %rdx
    divq %rbx
    addb $'0', %dl
    pushq %rdx
    incq %rcx
    testq %rax, %rax
    jnz convert_loop

    movq %rcx, %rdx
    movq %rsi, %rdi

store_loop:
    testq %rcx, %rcx
    jz store_done
    popq %rax
    movb %al, (%rdi)
    incq %rdi
    decq %rcx
    jmp store_loop

store_done:
    movb $0, (%rdi)
    movq %rdi, %rax
    subq %r12, %rax

ltoa_exit:
    popq %r12
    popq %rbx
    movq %rbp, %rsp
    popq %rbp
    ret

fibonacci:
    pushq %rbp
    movq %rsp, %rbp
    pushq %rbx
    pushq %r12
    pushq %r13
    pushq %r14

    movq %rdi, %r12

    leaq array(%rip), %rcx
    movq %rdi, %rax
    shlq $4, %rax
    addq %rax, %rcx
    movq (%rcx), %rax
    movq 8(%rcx), %rdx
    orq %rdx, %rax
    jnz fib_done

    cmpq $2, %rdi
    jg recursive_case
    movq $1, (%rcx)
    movq $0, 8(%rcx)
    movq $1, %rax
    movq $0, %rdx
    jmp fib_return

recursive_case:
    decq %rdi
    call fibonacci
    movq %rax, %r13
    movq %rdx, %r14

    movq %r12, %rdi
    subq $2, %rdi
    call fibonacci

    addq %r13, %rax
    movq %rdx, %rbx
    adcq %r14, %rbx

    leaq array(%rip), %rcx
    movq %r12, %r13
    shlq $4, %r13
    addq %r13, %rcx
    movq %rax, (%rcx)
    movq %rbx, 8(%rcx)
    movq %rbx, %rdx

fib_return:
fib_done:
    popq %r14
    popq %r13
    popq %r12
    popq %rbx
    movq %rbp, %rsp
    popq %rbp
    ret

exit:
    movq $60, %rax
    xorq %rdi, %rdi
    syscall
