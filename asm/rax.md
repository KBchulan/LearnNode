
![register](https://i-blog.csdnimg.cn/blog_migrate/2fff47d81ef1ae2385e62e0ad0522197.png)

%rax 存储的系统调用标识符：

文件操作：  
0 对应 read（从文件中读取数据）。  
1 对应 write（向文件中写入数据）。  
2 对应 open（打开文件）。  
3 对应 close（关闭文件）。  

进程控制：  
60 对应 exit（退出进程）。  
61 对应 fork（创建一个新进程）。  
62 对应 vfork（创建一个新进程，但不复制父进程的内存空间）。  
63 对应 clone（创建与父进程共享某些资源的线程）。  


内存管理：  
9 对应 mmap（内存映射文件）。  
11 对应 munmap（解除内存映射）。  
12 对应 brk（调整数据段的大小）。  


文件系统：  
5 对应 lseek（移动文件指针）。  
6 对应 readlink（读取符号链接的目标）。  
21 对应 mkdir（创建目录）。  
 rmdir 对应 rmdir（删除目录）。  

时间操作：  
228 对应 clock_gettime（获取指定时钟的时间）。  

网络操作：  
socket：创建套接字。  
bind：绑定地址到套接字。  
listen：监听连接请求。  
accept：接受连接请求。  
