#include "MutexDeadLock.hpp"

#include <atomic>
#include <cstdint>
#include <functional>
#include <global/Global.hpp>
#include <middleware/Logger.hpp>
#include <mutex>
#include <thread>

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
    // 所有的锁
    // mutexCall();

    // 锁包装器
    // lockGuardCall();
    uniqueLockCall();
  }
}

void MutexDeadLock::mutexCall() noexcept {
  // 互斥锁的思想：任何时刻只允许一个线程持有锁并访问共享资源，其他线程尝试获取已经被其他线程持有的锁时会被阻塞直到锁被释放
  std::mutex mtx;
  std::atomic<std::uint32_t> num = 0;

  auto func = [](std::atomic<std::uint32_t>& num_, std::mutex& mtx_) -> void {
    while (num_.load() < 100) {
      // 阻塞式的加锁和解锁，也可以try_lock，仅仅尝试获取锁而不阻塞
      mtx_.lock();

      if (num_.load() < 100) {
        num_.store(num_.load() + 1);
      } else {
        mtx_.unlock();
        break;
      }

      logger.info("My thread id is: {}, and the num is: {}", globalVariable.hasher(std::this_thread::get_id()), num_.load());

      mtx_.unlock();
    }
  };

  std::jthread thr_1(func, std::ref(num), std::ref(mtx));
  std::jthread thr_2(func, std::ref(num), std::ref(mtx));
}

void MutexDeadLock::lockGuardCall() noexcept {
  std::mutex mtx;
  std::atomic<std::uint32_t> num = 0;

  auto func = [](std::atomic<std::uint32_t>& num_, std::mutex& mtx_) -> void {
    while (num_.load() < 100) {
      // 简单的锁包装器，源码也是非常简单，利用RAII实现的自动加锁和解锁
      std::lock_guard<std::mutex> lock_guard(mtx_);

      if (num_.load() < 100) {
        num_.store(num_.load() + 1);
      } else {
        break;
      }

      logger.info("My thread id is: {}, and the num is: {}", globalVariable.hasher(std::this_thread::get_id()), num_.load());
    }
  };

  std::jthread thr_1(func, std::ref(num), std::ref(mtx));
  std::jthread thr_2(func, std::ref(num), std::ref(mtx));
}

void MutexDeadLock::uniqueLockCall() noexcept {
  /*
    unique_lock: https://en.cppreference.com/w/cpp/thread/unique_lock
    这个锁支持不少东西，可以看一下源码或者reference，这里简单介绍一下用法：

    构造：移动语义相关的，传入一个锁会走自动加锁，第二个位置可选的比较多，如果传入一个时间段，则会try_lock_for，时间点则会try_lock_until，也可以传入std::defer_lock，
    std::try_to_lock, std::adopt_lock等

    除此以外，基本的函数：
      lock()：加锁
      mutex()：只读获取实际互斥锁的指针
      owns_lock()：判断是否持有锁
      try_lock()：非阻塞式的尝试加锁
      try_lock_for()：在传入的时间段内都尝试加锁
      try_lock_until()：在指定的时间点前都尝试加锁
      unlock()：解锁
      release()：让出实际互斥量资源指针，自身不占有
      swap()：交换两个unique_lock的资源对象(锁的本身和owns信息)
  */
  std::mutex mtx1;
  std::mutex mtx2;
  std::atomic<std::uint32_t> num1 = 0;
  std::atomic<std::uint32_t> num2 = 0;

  auto func = [](std::atomic_uint32_t& num1_, std::atomic_uint32_t& num2_, std::mutex& mtx1_, std::mutex& mtx2_) -> void {
    while (num1_.load() < 100 || num2_.load() < 100) {
      std::unique_lock<std::mutex> lock1(mtx1_, std::defer_lock);
      std::unique_lock<std::mutex> lock2(mtx2_, std::defer_lock);
      std::lock(lock1, lock2);
      
      if (num1_.load() < 100) {
        num1_.store(num1_.load() + 1);
      } else {
        break;
      }

      if (num2_.load() < 100) {
        num2_.store(num2_.load() + 1);
      } else {
        break;
      }

      logger.info( "My thread id is: {}, and the num1 is: {}, and the num2 is: {}", globalVariable.hasher(std::this_thread::get_id()), num1_.load(), num2_.load());
    }
  };

  std::jthread thr_1(func, std::ref(num1), std::ref(num2), std::ref(mtx1), std::ref(mtx2));
  std::jthread thr_2(func, std::ref(num1), std::ref(num2), std::ref(mtx1), std::ref(mtx2));
}

void MutexDeadLock::scopedLockCall() noexcept {}

}  // namespace core