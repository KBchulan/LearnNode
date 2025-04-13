#include "MutexDeadLock.hpp"

#include <cstdint>
#include <mutex>
#include <thread>

#include <global/Global.hpp>
#include <middleware/Logger.hpp>

namespace core {

void MutexDeadLock::enterFunc() noexcept {
  /*
    先介绍一下常见的锁：
      . 互斥锁(mutex)
      . 读写锁(Reader-Writer Lock)
      . 递归锁(Recursive Mutex)
      . 条件变量(Condition Variable)
      . 自旋锁(Spin Lock)
      . 信号量(Semaphore)
      . 原子操作(Atomic Operations)
      . 超时锁(Timed Mutex)
      . 共享锁(Shared Lock)

      . 悲观锁(Pessimistic Lock)
      . 乐观锁(Optimistic Lock)

      . 读写意向锁
  */
  {
    mutexCall();
  }
}

void MutexDeadLock::mutexCall() noexcept {
  // 互斥锁的思想：任何时刻只允许一个线程持有锁并访问共享资源，其他线程尝试获取已经被其他线程持有的锁时会被阻塞直到锁被释放
  std::mutex mtx;
  std::uint32_t num = 0;

  auto increment = [](std::uint32_t& num_, std::mutex& mtx_) -> void {
    mtx_.lock();
    while (num_ < 100) {
      num_++;
      logger.info("My thread id is: {}, and the num is: {}",
                  globalVariable.hasher(std::this_thread::get_id()), num_);
    }
    mtx_.unlock();
  };

  std::jthread thr_1(increment, std::ref(num), std::ref(mtx));
  std::jthread thr_2(increment, std::ref(num), std::ref(mtx));
}

}  // namespace core