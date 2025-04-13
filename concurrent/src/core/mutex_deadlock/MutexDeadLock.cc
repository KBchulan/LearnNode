#include "MutexDeadLock.hpp"

#include <cstdint>
#include <functional>
#include <mutex>
#include <thread>

#include <global/Global.hpp>
#include <middleware/Logger.hpp>

namespace core {

void MutexDeadLock::enterFunc() noexcept {
  /*
    先介绍一下常见的锁：
      1. 乐观锁(Optimistic Lock)
      2. 悲观锁(Pessimistic Lock)
      3. 互斥锁(mutex)
      4. 读写锁(Reader-Writer Lock)
      5. 自旋锁(Spin Lock)
      6. 条件变量(Condition Variable)
      7. 递归锁(Recursive Mutex)
      8. 信号量(Semaphore)
      9. 原子操作(Atomic Operations)
      10. 超时锁(Timed Mutex)
      11. 共享锁(Shared Lock)
  */
  {
    mutexCall();
  }
}

void MutexDeadLock::mutexCall() noexcept {
  std::mutex mtx;
  std::uint32_t num = 1;

  auto increment = [](std::uint32_t& num, std::mutex& mtx) -> void {
    while (num <= 10000) {
      mtx.lock();
      num++;
      logger.info("My thread id is: {}, and the num is: {}", std::this_thread::get_id(), num);
      mtx.unlock();
    }
  };

  std::jthread thr_1(increment, std::ref(num), std::ref(mtx));
  std::jthread thr_2(increment, std::ref(num), std::ref(mtx));
}

}  // namespace core