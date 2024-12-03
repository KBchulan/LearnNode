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
    .comm SI, 8            # 关键字指针
    .comm DI, 8            # 句子中匹配位置指针
    .comm CX, 8            # 关键字长度
    .comm BX, 8            # 当前比较位置

.section .text
.globl _start

_start:
    # 读取关键字
    movq $prompt1, %rsi
    call print_string
    movq $keyword, %rsi
    call read_string
    
    # 计算关键字长度
    movq $keyword, %rdi
    call strlen
    movq %rax, CX         # 保存关键字长度
    
sentence_loop:
    # 读取句子
    movq $prompt2, %rsi
    call print_string
    movq $sentence, %rsi
    call read_string
    
    # 初始化搜索
    movq $keyword, %rsi    # SI = 关键字起始地址
    movq $sentence, %rdi   # DI = 句子起始地址
    movq $0, %rbx         # BX = 当前比较位置
    
search_loop:
    # 比较字符
    movb (%rdi,%rbx), %al
    testb %al, %al
    jz not_found          # 到达句子末尾，未找到
    
    pushq %rdi
    pushq %rsi
    pushq %rbx
    
    # 从当前位置开始比较字符串
    leaq (%rdi,%rbx), %rdi
    movq CX, %rdx         # 比较长度
    call strncmp
    
    popq %rbx
    popq %rsi
    popq %rdi
    
    testq %rax, %rax
    jz found              # 找到匹配
    
    incq %rbx
    jmp search_loop

found:
    movq $match_msg, %rsi
    call print_string
    movq %rbx, %rax
    incq %rax             # 位置从1开始计数
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

# 字符串长度计算函数
strlen:
    xorq %rax, %rax       # 计数器清零
strlen_loop:
    cmpb $0, (%rdi,%rax)
    je strlen_done
    incq %rax
    jmp strlen_loop
strlen_done:
    ret

# 字符串比较函数
# %rdi = 字符串1, %rsi = 字符串2, %rdx = 长度
strncmp:
    xorq %rcx, %rcx       # 计数器清零

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
    xorq %rax, %rax       # 返回0表示相等
    ret
strncmp_notequal:
    movq $1, %rax         # 返回1表示不相等
    ret

# 字符串打印函数
print_string:
    pushq %rsi            # 保存字符串指针
    movq %rsi, %rdi
    call strlen           # 获取字符串长度
    movq %rax, %rdx       # 长度作为参数
    popq %rsi             # 恢复字符串指针
    movq $1, %rax         # sys_write
    movq $1, %rdi         # stdout
    syscall
    ret

# 字符串读取函数
read_string:
    movq $0, %rax        # sys_read
    movq $0, %rdi        # stdin
    movq $1000, %rdx     # 最大读取长度
    syscall
    
    # 将换行符替换为null
    movq %rsi, %rdi
    decq %rax            # 跳过换行符
    movb $0, (%rdi,%rax)
    ret

# 数字打印函数
print_number:
    movq $buffer+19, %rsi  # 缓冲区末尾
    movb $0, (%rsi)       # null结束符
    movq $10, %rcx        # 除数
print_number_loop:
    decq %rsi
    xorq %rdx, %rdx       # 清除余数
    divq %rcx             # 除以10
    addb $'0', %dl        # 转换为ASCII
    movb %dl, (%rsi)
    testq %rax, %rax
    jnz print_number_loop
    call print_string     # 打印结果
    ret
    