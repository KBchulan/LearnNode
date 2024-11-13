.section .data
    # 常量定义
    .equ MAX_ENTRIES, 50
    .equ NAME_SIZE, 20
    .equ TEL_SIZE, 8
    
    # 提示信息
    msg_name:    .string "Input name: "
    msg_tel:     .string "Input a telephone number: "
    msg_menu:    .string "\nPlease choose:\n1. Continue input\n2. Search record\n3. Exit\nYour choice: "
    msg_ask:     .string "Input the name to search: "
    msg_header:  .string "name\t\ttel.\n"
    msg_notfound:.string "Not found!\n"
    msg_newline: .string "\n"
    msg_comma:   .string ","
    
    # 文件相关
    filename:    .string "table.txt"
    
    # 缓冲区
    .align 16
    buffer:      .space 100
    name_buf:    .space NAME_SIZE
    tel_buf:     .space TEL_SIZE
    file_buf:    .space 1024

.section .text
.globl _start

# 函数：打印字符串，参数：%rsi=字符串地址
prints:
    pushq %rbp
    movq %rsp, %rbp
    # 计算字符串长度
    movq %rsi, %rdi
    xorq %rcx, %rcx
    not %rcx
    xorb %al, %al
    repne scasb
    not %rcx
    dec %rcx
    # 打印字符串
    movq $1, %rax
    movq $1, %rdi
    movq %rcx, %rdx
    syscall
    leave
    ret

# 函数：读取输入，参数：%rsi=缓冲区地址，%rdx=最大长度
reads:
    pushq %rbp
    movq %rsp, %rbp
    movq $0, %rax
    movq $0, %rdi
    syscall
    # 去除换行符
    decq %rax
    movb $0, (%rsi,%rax)
    leave
    ret

# 函数：写入文件
write_csv:
    pushq %rbp
    movq %rsp, %rbp
    
    # 打开文件
    movq $2, %rax           # sys_open
    movq $filename, %rdi
    movq $0102, %rsi        # O_CREAT | O_APPEND
    movq $0666, %rdx        # 权限
    syscall
    
    movq %rax, %r12        # 保存文件描述符
    
    # 准备写入数据
    movq $1, %rax          # sys_write
    movq %r12, %rdi
    movq $name_buf, %rsi   # 写入名字
    movq $NAME_SIZE, %rdx
    syscall
    
    movq $1, %rax
    movq %r12, %rdi
    movq $msg_comma, %rsi  # 写入逗号
    movq $1, %rdx
    syscall
    
    movq $1, %rax
    movq %r12, %rdi
    movq $tel_buf, %rsi    # 写入电话
    movq $TEL_SIZE, %rdx
    syscall
    
    movq $1, %rax
    movq %r12, %rdi
    movq $msg_newline, %rsi # 写入换行
    movq $1, %rdx
    syscall
    
    # 关闭文件
    movq $3, %rax
    movq %r12, %rdi
    syscall
    
    leave
    ret

_start:
menu:
    # 显示菜单
    movq $msg_menu, %rsi
    call prints
    
    # 读取选择
    movq $buffer, %rsi
    movq $2, %rdx
    call reads
    
    # 检查选择
    cmpb $'1', (%rsi)
    je input_loop
    cmpb $'2', (%rsi)
    je search_loop
    cmpb $'3', (%rsi)
    je exit_prog
    jmp menu

input_loop:
    # 输入姓名
    movq $msg_name, %rsi
    call prints
    
    # 读取姓名
    movq $name_buf, %rsi
    movq $NAME_SIZE, %rdx
    call reads
    
    # 输入电话号码
    movq $msg_tel, %rsi
    call prints
    
    # 读取电话号码
    movq $tel_buf, %rsi
    movq $TEL_SIZE, %rdx
    call reads
    
    # 写入文件
    call write_csv
    
    # 返回菜单
    jmp menu

search_loop:
    # 输入要查找的姓名
    movq $msg_ask, %rsi
    call prints
    
    movq $buffer, %rsi
    movq $NAME_SIZE, %rdx
    call reads
    
    # 打开文件查找
    movq $2, %rax          # sys_open
    movq $filename, %rdi
    movq $0, %rsi          # O_RDONLY
    syscall
    
    movq %rax, %r12        # 保存文件描述符
    
    # 读取文件内容
    movq $0, %rax          # sys_read
    movq %r12, %rdi
    movq $file_buf, %rsi
    movq $1024, %rdx
    syscall
    
    # 在缓冲区中查找名字
    movq $file_buf, %rdi
    movq $buffer, %rsi
    call search_in_buffer
    
    # 关闭文件
    movq $3, %rax
    movq %r12, %rdi
    syscall
    
    # 返回菜单
    jmp menu

# 在缓冲区中搜索名字并处理未找到的情况
search_in_buffer:
    pushq %rbp
    movq %rsp, %rbp
    
    # 检查是否找到
    cmpb $0, (%rdi)
    je not_found
    
    # 打印表头
    movq $msg_header, %rsi
    call prints
    
    # 打印找到的结果
    movq %rdi, %rsi
    call prints
    jmp search_done
    
not_found:
    movq $msg_notfound, %rsi
    call prints
    
search_done:
    leave
    ret

exit_prog:
    movq $60, %rax
    xorq %rdi, %rdi
    syscall
    