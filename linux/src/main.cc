/*
  Linux API：内存管理， 进程管理， 文件系统， "设备管理"， 网络管理

  内存管理：
    - brk()：调整进程的堆内存大小
    - mmap()：将文件的内容映射到内存区域进行访问

  进程管理：
    - fork(): 创建一个新的进程
    - sched_yield(): 让出CPU时间片
    - exit(), wait()等系列函数见Process.md
  
  文件系统：
    - open(), read(), write(), close()
  
  设备管理：
    
  网络管理：
    - socket(), bind(), listen(), accept(), connect(), send(), recv()：网络基础api
    - socketpair(): 创建一对匿名管道
*/

#include <Memory/Memory.hpp>
#include <Process/Process.hpp>
#include <Vector2.hpp>
#include <global/Global.hpp>
#include <middleware/Logger.hpp>

int main() {
  {
    // globalVariable
    // logger.info("globalVariable: {}", globalVariable.redis_config_.host);
    // logger.info("globalVariable: {}", globalVariable.redis_config_.port);
    // logger.info("globalVariable: {}", globalVariable.redis_config_.password);
  }

  {
    // memory
    // core::Memory::brkCall();
    // core::Memory::mmapCall();
  }

  {
    // process
    // core::Process::forkCall();
    // core::Process::sched_yieldCall();
    // core::Process::shmCall();
  }

}
