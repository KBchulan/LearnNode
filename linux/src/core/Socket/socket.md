# Linux进程间通信机制

## Socket（套接字）

Socket是一种允许不同进程在网络环境或同一台计算机上进行通信的机制。Socket是位于应用层与传输层之间的抽象层，它屏蔽了不同网络协议之间的差异，为程序提供了统一的接口。

### 主要特点
- 支持跨网络、跨主机的通信
- 提供多种通信方式：面向连接(TCP)和无连接(UDP)
- 可以实现多种通信模式：一对一、一对多、多对多等

### 基本API
```c
// 创建socket
int socket(int domain, int type, int protocol);

// 绑定地址
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

// 监听连接（仅TCP）
int listen(int sockfd, int backlog);

// 接受连接（仅TCP）
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

// 发起连接（仅TCP）
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

// 发送数据
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);

// 接收数据
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
```

### 示例代码（TCP服务器和客户端）

服务器端：
```c
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
    
    // 创建socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        return 1;
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);
    
    // 绑定地址
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return 1;
    }
    
    // 监听连接
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        return 1;
    }
    
    // 接受连接
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        return 1;
    }
    
    // 接收数据
    read(new_socket, buffer, 1024);
    printf("收到消息: %s\n", buffer);
    
    // 发送数据
    send(new_socket, hello, strlen(hello), 0);
    printf("发送问候消息\n");
    
    close(new_socket);
    close(server_fd);
    return 0;
}
```

客户端：
```c
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    
    // 创建socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return 1;
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    
    // 转换IP地址
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address");
        return 1;
    }
    
    // 连接服务器
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        return 1;
    }
    
    // 发送数据
    send(sock, hello, strlen(hello), 0);
    printf("发送问候消息\n");
    
    // 接收数据
    read(sock, buffer, 1024);
    printf("收到消息: %s\n", buffer);
    
    close(sock);
    return 0;
}
```

## Pipe（管道）

管道是Unix/Linux系统中最古老的IPC机制之一，提供了一种简单的方式来实现单向数据流。

### 主要特点
- 只能单向传输数据（单工通信）
- 通常用于相关进程间通信（如父子进程）
- 管道是临时的，只存在于使用它的进程生命周期内

### 基本API
```c
// 创建管道
int pipe(int pipefd[2]);
```
其中`pipefd[0]`用于读取，`pipefd[1]`用于写入。

### 示例代码
```c
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main() {
    int pipefd[2];
    pid_t pid;
    char buf[100];
    
    // 创建管道
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }
    
    pid = fork();
    
    if (pid == -1) {
        perror("fork");
        return 1;
    }
    
    if (pid == 0) {    // 子进程
        close(pipefd[1]);  // 关闭写端
        
        // 从管道读取数据
        read(pipefd[0], buf, sizeof(buf));
        printf("子进程收到: %s\n", buf);
        
        close(pipefd[0]);  // 关闭读端
    } else {           // 父进程
        close(pipefd[0]);  // 关闭读端
        
        // 向管道写入数据
        write(pipefd[1], "从父进程发送的消息", strlen("从父进程发送的消息"));
        
        close(pipefd[1]);  // 关闭写端
        wait(NULL);        // 等待子进程结束
    }
    
    return 0;
}
```

## Socketpair（套接字对）

Socketpair是创建一对相互连接的套接字的系统调用，主要用于实现全双工通信。

### 主要特点
- 提供全双工通信（双向数据流）
- 仅用于相关进程间通信，通常是父子进程
- 比使用两个管道实现双向通信更简洁

### 基本API
```c
int socketpair(int domain, int type, int protocol, int sv[2]);
```

### 示例代码
```c
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main() {
    int sv[2]; // 套接字对
    char buf[100];
    
    // 创建socketpair
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == -1) {
        perror("socketpair");
        return 1;
    }
    
    pid_t pid = fork();
    
    if (pid > 0) { // 父进程
        close(sv[1]); // 关闭一个描述符
        write(sv[0], "父进程发送的消息", strlen("父进程发送的消息"));
        read(sv[0], buf, sizeof(buf));
        printf("父进程收到: %s\n", buf);
        close(sv[0]);
    } else if (pid == 0) { // 子进程
        close(sv[0]); // 关闭一个描述符
        read(sv[1], buf, sizeof(buf));
        printf("子进程收到: %s\n", buf);
        write(sv[1], "子进程发送的消息", strlen("子进程发送的消息"));
        close(sv[1]);
    }
    
    return 0;
}
```

## FIFO（命名管道）

FIFO是一种特殊类型的文件，允许不相关的进程通过文件系统进行通信。

### 主要特点
- 提供持久化的管道通信
- 允许不相关进程间通信
- 具有文件系统路径标识

### 基本API
```c
// 创建FIFO
int mkfifo(const char *pathname, mode_t mode);
```

### 示例代码
```c
// 写入FIFO的进程
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main() {
    int fd;
    char *message = "通过FIFO发送的消息";
    
    // 创建FIFO
    mkfifo("/tmp/myfifo", 0666);
    
    // 打开FIFO写入
    fd = open("/tmp/myfifo", O_WRONLY);
    write(fd, message, strlen(message) + 1);
    close(fd);
    
    return 0;
}
```

```c
// 从FIFO读取的进程
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int fd;
    char buf[100];
    
    // 打开FIFO读取
    fd = open("/tmp/myfifo", O_RDONLY);
    read(fd, buf, sizeof(buf));
    printf("收到: %s\n", buf);
    close(fd);
    
    // 删除FIFO
    unlink("/tmp/myfifo");
    
    return 0;
}
```

## 共享内存

共享内存是最快的IPC机制，允许多个进程访问同一块内存区域。

### 主要特点
- 数据传输速度最快
- 需要同步机制（如信号量）来协调访问
- 适合大量数据交换的场景

### 系统V共享内存API
```c
// 创建/获取共享内存段
int shmget(key_t key, size_t size, int shmflg);

// 附加共享内存段到进程地址空间
void *shmat(int shmid, const void *shmaddr, int shmflg);

// 从进程分离共享内存段
int shmdt(const void *shmaddr);

// 控制共享内存段
int shmctl(int shmid, int cmd, struct shmid_ds *buf);
```

### POSIX共享内存API
```c
// 创建/打开共享内存对象
int shm_open(const char *name, int oflag, mode_t mode);

// 调整共享内存对象大小
int ftruncate(int fd, off_t length);

// 删除共享内存对象
int shm_unlink(const char *name);
```

### 示例代码（POSIX共享内存）
```c
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

int main() {
    int fd;
    void *ptr;
    
    // 创建共享内存对象
    fd = shm_open("/myshm", O_CREAT | O_RDWR, 0666);
    
    // 设置大小
    ftruncate(fd, 4096);
    
    // 映射到进程地址空间
    ptr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return 1;
    }
    
    // 写入数据
    sprintf((char*)ptr, "共享内存中的数据");
    
    // fork子进程
    if (fork() == 0) {
        // 子进程读取数据
        printf("子进程读取: %s\n", (char*)ptr);
        
        // 子进程修改数据
        sprintf((char*)ptr, "子进程修改后的数据");
        
        // 解除映射
        munmap(ptr, 4096);
        close(fd);
        
        return 0;
    }
    
    // 父进程等待
    sleep(1);
    
    // 父进程读取修改后的数据
    printf("父进程读取: %s\n", (char*)ptr);
    
    // 解除映射并删除共享内存对象
    munmap(ptr, 4096);
    close(fd);
    shm_unlink("/myshm");
    
    return 0;
}
```

## 消息队列

消息队列允许进程通过发送和接收消息来交换数据。

### 主要特点
- 支持消息的类型化和优先级
- 提供异步通信机制
- 消息在被读取前保存在内核中

### 系统V消息队列API
```c
// 创建/获取消息队列
int msgget(key_t key, int msgflg);

// 发送消息
int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);

// 接收消息
ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);

// 控制消息队列
int msgctl(int msqid, int cmd, struct msqid_ds *buf);
```

### POSIX消息队列API
```c
// 创建/打开消息队列
mqd_t mq_open(const char *name, int oflag, mode_t mode, struct mq_attr *attr);

// 发送消息
int mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned int msg_prio);

// 接收消息
ssize_t mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned int *msg_prio);

// 关闭消息队列
int mq_close(mqd_t mqdes);

// 删除消息队列
int mq_unlink(const char *name);
```

## 总结与对比

| IPC机制 | 通信方向 | 相关性要求 | 持久性 | 速度 | 适用场景 |
|---------|----------|------------|--------|------|----------|
| Socket | 双工 | 不相关 | 临时 | 中等 | 网络通信、不相关进程间通信 |
| Pipe | 单工 | 相关 | 临时 | 高 | 父子进程简单通信 |
| Socketpair | 双工 | 相关 | 临时 | 高 | 父子进程双向通信 |
| FIFO | 单工 | 不相关 | 持久 | 高 | 不相关进程间单向通信 |
| 共享内存 | 双工 | 不相关 | 持久 | 最高 | 大量数据交换 |
| 消息队列 | 双工 | 不相关 | 持久 | 中等 | 异步通信、类型化消息 |

## 选择建议

1. 需要最高性能：选择共享内存
2. 需要简单的父子进程通信：使用管道
3. 需要全双工的父子进程通信：使用socketpair
4. 需要网络通信能力：使用socket
5. 需要在不相关进程间进行简单通信：使用FIFO
6. 需要结构化消息和优先级：使用消息队列
