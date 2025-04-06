#include "Process.hpp"

#include <cerrno>
#include <fcntl.h>
#include <cstdlib>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include <model/Model.hpp>
#include <middleware/Logger.hpp>

namespace core {

/*
  fork：Linux中创建进程的方式
    - 调用：pid_t fork(void);
    - 介绍：
        - 当一个进程调用fork()时，会创建一个新的进程，这个新进程是父进程的一个副本，会获得父进程的数据段以及堆栈的副本并共享.rodata端
          两个进程同时从fork()的调用处继续执行，但是是有不同的pid的
        - return：返回值为负数，则是创建失败，0表示子进程执行，正数则是父进程在执行(这个正数是子进程的pid)
    - 注意：fork的实现是通过copy-on-write实现的，即父子进程是共享物理内存的，只有共享内存要被修改时才会创建副本
*/
void Process::forkCall() noexcept {
  // 创建并行任务
  auto parallel = []() -> void {
    pid_t pid = fork();

    if (pid < 0) {
      logger.info("子进程创建失败, pid is: {}", pid);
    }
    else if (pid == 0){
      logger.info("我是子进程, pid is: {}", getpid());
      _exit(0);
    }
    else {
      logger.info("我是父进程, 我的子进程pid is: {}", pid);
      wait(nullptr);  // wait son process finish
    }
  };

  // 创建守护进程
  auto guard = []() -> void {
    pid_t pid = fork();

    if(pid > 0) {
      // 出现问题时父进程退出
      exit(0);
    }

    // 子进程会成为守护进程
    setsid();
  };

  parallel();
  guard();
}

/*
  sched_yield：Linux中进程主动放弃CPU时间片
    - 调用：int sched_yield(void);
    - 介绍：
      - return：成功为0,失败返回-1(同时设置errno值)
      - 当进程调用sched_yield时，内核会将该进程从当前CPU的运行队列中移除，然后将这个进程置于同优先级队列的末尾，接着正常执行所有进程
      - 当一个进程在等待某个资源短暂可用时，可以使用sched_yield而不是忙等待
      - 避免CPU的独占，可以进行周期性调用
*/
void Process::sched_yieldCall() noexcept {
  logger.info("执行一段计算进程, pid: {}", getpid());

  for (int i = 0; i < 5; i++) {
    logger.info("当前为任务{}", i);
    for(int j = 0; j < 50000000; j++) {
      if(j % 20000000 == 0) {
        logger.info("当前值为:{} ", j);
        sched_yield();
      }
    }
  }
}

void Process::shmCall() noexcept {
  int file_des = shm_open("/shm_obj", O_RDWR | O_CREAT | O_TRUNC, 0666);

  if (file_des < 0) {
    logger.info("shm open error, errno: {}", errno);
    return;
  }

  if (ftruncate(file_des, sizeof(model::SharedMemory)) < 0) {
    logger.info("ftruncate error, errno: {}", errno);
    return;
  }

  auto *shmptr = reinterpret_cast<model::SharedMemory *>(mmap(nullptr, sizeof(model::SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, file_des, 0));

  if (shmptr == MAP_FAILED) {
    logger.info("mmap error, errno: {}", errno);
    return;
  }
  
  close(file_des);

  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);
  pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
  pthread_mutex_init(&shmptr->mtx, &attr);

  std::uint64_t counts = 0;

  // 引入一个子进程
  pid_t pid_cpy = fork();
  if (pid_cpy < 0) {
    logger.info("fork error, errno: {}", errno);
    return;
  }
  if (pid_cpy == 0) {
    while (counts < 1000 *1000) {
      {
        pthread_mutex_lock(&shmptr->mtx);
        shmptr->counts += 1;
        pthread_mutex_unlock(&shmptr->mtx);
      }
      counts++;
    }
    exit(0);
  }
  else {
    while (counts < 1000 * 1000) {
      {
        pthread_mutex_lock(&shmptr->mtx);
        shmptr->counts += 1;
        pthread_mutex_unlock(&shmptr->mtx);
      }
      counts++;
    }
    wait(nullptr);
    logger.info("父子进程计算结果为: {}", shmptr->counts);
  }
}

} // namespace core