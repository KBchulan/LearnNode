# Linux进程管理API

## 1. 进程终止：exit系列函数

### 1.1 exit()

```c
#include <stdlib.h>
void exit(int status);
```

- 终止当前进程，返回状态码给父进程
- 调用所有通过atexit()注册的函数
- 刷新和关闭所有打开的流
- 执行标准I/O库的清理操作

### 1.2 _exit() / _Exit()

```c
#include <unistd.h>
void _exit(int status);

#include <stdlib.h>
void _Exit(int status);
```

- 立即终止进程，不执行任何清理操作
- 不调用atexit()注册的函数
- 不刷新流缓冲区
- 通常在fork后的子进程中使用，避免执行父进程注册的清理函数

## 2. 进程等待：wait系列函数

### 2.1 wait()

```c
#include <sys/wait.h>
pid_t wait(int *status);
```

- 阻塞调用进程直到任意一个子进程终止
- 返回终止子进程的PID
- 通过status参数返回子进程的退出状态

### 2.2 waitpid()

```c
#include <sys/wait.h>
pid_t waitpid(pid_t pid, int *status, int options);
```

- 可以等待特定子进程（由pid指定）
- options参数允许非阻塞等待(WNOHANG)
- 提供更多控制来处理子进程终止

### 2.3 waitid()

```c
#include <sys/wait.h>
int waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options);
```

- 比waitpid()更灵活，允许按进程组ID等方式等待
- 提供更详细的终止信息

### 2.4 状态宏

用于检查子进程终止状态的宏：
- `WIFEXITED(status)`：子进程正常终止时返回非零值
- `WEXITSTATUS(status)`：返回子进程exit()的参数值
- `WIFSIGNALED(status)`：子进程被信号终止时返回非零值
- `WTERMSIG(status)`：返回终止子进程的信号编号
- `WCOREDUMP(status)`：子进程产生核心转储时返回非零值
- `WIFSTOPPED(status)`：子进程暂停时返回非零值
- `WSTOPSIG(status)`：返回引起子进程暂停的信号编号

## 3. 执行程序：exec系列函数

### 3.1 execl(), execlp(), execle()

```c
#include <unistd.h>
int execl(const char *path, const char *arg, ... /*, NULL */);
int execlp(const char *file, const char *arg, ... /*, NULL */);
int execle(const char *path, const char *arg, ... /*, NULL, char *const envp[] */);
```

### 3.2 execv(), execvp(), execvpe()

```c
#include <unistd.h>
int execv(const char *path, char *const argv[]);
int execvp(const char *file, char *const argv[]);
int execvpe(const char *file, char *const argv[], char *const envp[]);
```

- 用新程序替换当前进程的映像
- 保持PID和打开的文件描述符不变
- 命名规则：
  - 带"l"：参数以列表形式提供
  - 带"v"：参数以数组形式提供
  - 带"p"：使用PATH环境变量查找可执行文件
  - 带"e"：允许指定新的环境变量

## 4. 进程控制与状态

### 4.1 getpid(), getppid()

```c
#include <unistd.h>
pid_t getpid(void);
pid_t getppid(void);
```

- 获取当前进程ID和父进程ID

### 4.2 nice()

```c
#include <unistd.h>
int nice(int inc);
```

- 改变进程优先级
- 参数inc为优先级增量，正值降低优先级，负值提高优先级

## 5. 高级进程创建

### 5.1 vfork()

```c
#include <unistd.h>
pid_t vfork(void);
```

- 类似fork()，但不完全复制父进程的地址空间
- 设计用于子进程立即执行exec的情况
- 子进程与父进程共享地址空间，子进程必须先于父进程执行exit或exec

### 5.2 clone()

```c
#include <sched.h>
int clone(int (*fn)(void *), void *stack, int flags, void *arg, ...);
```

- Linux特有的系统调用
- 比fork()更灵活，可以精确控制子进程与父进程共享的资源
- 用于实现线程、命名空间等高级功能

### 5.3 posix_spawn()

```c
#include <spawn.h>
int posix_spawn(pid_t *pid, const char *path,
               const posix_spawn_file_actions_t *file_actions,
               const posix_spawnattr_t *attrp,
               char *const argv[], char *const envp[]);
```

- 结合fork和exec功能，更加高效
- 特别适用于资源受限环境

## 6. 进程调度控制

### 6.1 sched_setscheduler()

```c
#include <sched.h>
int sched_setscheduler(pid_t pid, int policy, const struct sched_param *param);
```

- 设置进程的调度策略和参数
- 策略包括：SCHED_OTHER(普通)、SCHED_FIFO(实时)、SCHED_RR(实时轮转)等

### 6.2 sched_setparam()

```c
#include <sched.h>
int sched_setparam(pid_t pid, const struct sched_param *param);
```

- 设置进程的调度参数，不改变调度策略

### 6.3 sched_get_priority_max/min()

```c
#include <sched.h>
int sched_get_priority_max(int policy);
int sched_get_priority_min(int policy);
```

- 获取指定调度策略支持的最大/最小优先级

### 6.4 sched_rr_get_interval()

```c
#include <sched.h>
int sched_rr_get_interval(pid_t pid, struct timespec *tp);
```

- 获取SCHED_RR策略的时间片长度

## 7. 进程组与会话

### 7.1 setpgid()

```c
#include <unistd.h>
int setpgid(pid_t pid, pid_t pgid);
```

- 设置进程的进程组ID

### 7.2 getpgrp()

```c
#include <unistd.h>
pid_t getpgrp(void);
```

- 获取调用进程的进程组ID

### 7.3 setsid()

```c
#include <unistd.h>
pid_t setsid(void);
```

- 创建新会话并设置调用进程为会话首进程
- 常用于创建守护进程

## 8. 实际应用示例

### 8.1 创建守护进程

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

void daemonize() {
    pid_t pid = fork();
    
    if (pid < 0) exit(1);
    if (pid > 0) exit(0);  // 父进程退出
    
    // 创建新会话
    if (setsid() < 0) exit(1);
    
    // 忽略某些信号
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
    
    // 第二次fork防止重新获取终端
    pid = fork();
    if (pid < 0) exit(1);
    if (pid > 0) exit(0);
    
    // 更改工作目录
    chdir("/");
    
    // 重定向标准文件描述符
    int fd = open("/dev/null", O_RDWR);
    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    if (fd > 2) close(fd);
}

int main() {
    daemonize();
    
    // 守护进程的实际工作
    while (1) {
        // 执行任务
        sleep(60);
    }
    
    return 0;
}
```

### 8.2 实时进程示例

```c
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    struct sched_param param;
    int max_prio;
    
    // 获取SCHED_FIFO的最高优先级
    max_prio = sched_get_priority_max(SCHED_FIFO);
    if (max_prio == -1) {
        perror("sched_get_priority_max");
        exit(1);
    }
    
    // 设置调度参数
    param.sched_priority = max_prio;
    
    // 设置为实时调度策略
    if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
        perror("sched_setscheduler");
        exit(1);
    }
    
    printf("运行在实时调度策略SCHED_FIFO下\n");
    
    // 执行实时关键任务
    for (int i = 0; i < 10; i++) {
        printf("执行实时任务 %d\n", i);
        usleep(100000);  // 模拟工作
    }
    
    return 0;
}
```
