#include "MutexDeadLock.hpp"

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <functional>
#include <global/Global.hpp>
#include <middleware/Logger.hpp>
#include <mutex>
#include <queue>
#include <random>
#include <shared_mutex>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <vector>

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
    // rwmutexCall();
    // recursivemutexCall();
    conditionVariableCall();

    // 锁包装器
    // lockGuardCall();
    // uniqueLockCall();
    // scopedLockCall();
    // sharedLockCall();
  }
}

void MutexDeadLock::mutexCall() noexcept {
  // 互斥锁的思想：任何时刻只允许一个线程持有锁并访问共享资源，其他线程尝试获取已经被其他线程持有的锁时会被阻塞直到锁被释放
  /*
    std::mutex: https://en.cppreference.com/w/cpp/thread/mutex

    基本的函数：
      lock()：加锁
      unlock()：解锁
      try_lock()：非阻塞式的尝试加锁
      native_handle()：获取实际的互斥锁的句柄
    
    从本质上来看，互斥锁有这么一个流程，当前线程尝试获取锁，如果该锁已经被其他线程占有，则此线程则会被投递到内核空间的
    等待队列(通过调用futex)，然后锁解开时会调用通知，释放等待队列，随后重新回到用户态，
    因此，多个上下文的切换导致的开销是很大的，且线程会被阻塞，这是不好的
  */
  std::mutex mtx;
  std::atomic<std::uint32_t> num = 0;

  auto func = [](std::atomic<std::uint32_t>& num_, std::mutex& mtx_) -> void {
    while (num_.load() < 100) {
      mtx_.lock();

      if (num_.load() < 100) {
        num_.fetch_add(1);
      } else {
        mtx_.unlock();
        break;
      }

      logger.info("My thread id is: {}, and the num is: {}", globalVariable.hasher(std::this_thread::get_id()), num_.load());

      mtx_.unlock();
    }
  };

  std::jthread thr_1{func, std::ref(num), std::ref(mtx)};
  std::jthread thr_2{func, std::ref(num), std::ref(mtx)};
}

void MutexDeadLock::rwmutexCall() noexcept {
  /*
    cppreference: https://en.cppreference.com/w/cpp/thread/shared_mutex
    事实上，临界资源的读操作是不会导致线程安全问题的，所以产生了读写锁，对这个进一步优化，允许多个线程同时读，但是写的时候只能一个线程写
    内部维护一个共享锁和一个独占锁，遵循以下原则：
      - 当有一个线程获取到独占锁时，任何线程都不能再获取共享锁和独占锁
      - 当有一个线程获取到共享锁时，任何线程都不能再获取独占锁，但可以获取共享锁
  */
  std::shared_mutex shared_mutex;
  // std::shared_timed_mutex shared_timed_mutex; 此处我们还有一个shared_timed_mutex，可以设置超时时间，经典的for和until，这里不演示了
  std::unordered_map<std::string, std::string> shared_entries;

  auto write = [&](const std::string& key, const std::string& value) -> void {
    shared_mutex.lock();

    shared_entries[key] = value;
    logger.info("write shared_entries, [{} is {}] ", key, value);

    shared_mutex.unlock();
  };

  auto read = [&](const std::string& key) -> void {
    shared_mutex.lock_shared();

    logger.info("read shared_entries, [{} is {}]", key, shared_entries[key]);
    shared_mutex.unlock_shared();
  };

  shared_entries["key1"] = "value1";
  shared_entries["key2"] = "value2";
  shared_entries["key3"] = "value3";

  std::jthread thr_1{read, "key1"};
  std::jthread thr_2{read, "key2"};
  std::jthread thr_3{write, "key4", "value4"};
}

void MutexDeadLock::recursivemutexCall() noexcept {
  /*
    cppreference: https://en.cppreference.com/w/cpp/thread/recursive_mutex
    同一线程多次获取同一把互斥锁时会导致死锁，可以使用递归锁避免这种情况的发生

    递归锁会记录拥有该锁的thread_id和加锁次数，同一线程重复获取会增加计数，其他线程获取则会被阻塞

    这个因为要跟踪所有者的信息，就导致其开销更多，会比普通的互斥锁更慢一些
  */
  std::recursive_mutex recumtx;
  std::uint32_t input = 10;
  // std::recursive_timed_mutex 一样的多一个时间参数的递归锁

  std::function<std::uint32_t(const std::uint32_t &)> fib = [&](const std::uint32_t &num) -> std::uint32_t {
    {
      std::scoped_lock<std::recursive_mutex> lock(recumtx);
      logger.info("thread id is: {}, n is: {}", globalVariable.hasher(std::this_thread::get_id()), num);
    }

    if (num <= 2) {
      return 1;
    }

    return fib(num - 1) + fib(num - 2);
  };

  std::vector<std::jthread> threads;
  threads.reserve(3);

  for (size_t i = 0; i < 3; i++) {
    threads.emplace_back(fib, std::ref(input));
  }
}

void MutexDeadLock::conditionVariableCall() noexcept {
  /*
    cppreference: https://en.cppreference.com/w/cpp/thread/condition_variable
    条件变量一般和互斥锁一起使用，用于阻塞一个或多个线程，同时通知一个或多个线程

    wait操作会在一个原子操作的时间内解锁并进入等待队列，进入休眠状态(这个状态是完全不消耗CPU资源的)，
    notify后进入就绪态，会尝试获取到锁，获取到锁后可以继续执行
  */
  std::mutex mtx;

  std::condition_variable data_cond;   // 通知有新数据
  std::condition_variable space_cond;  // 通知队列没有满
  // 传统的条件变量只能与std::mutex合作使用，这个any的可以与任何其他的锁联动使用
  // std::condition_variable_any

  std::queue<int> queue;
  size_t capacity = 10;
  bool done = false;

  auto push = [&](int num) -> void {
    std::unique_lock<std::mutex> lock(mtx, std::defer_lock);

    lock.lock();
    // 当队列不满或者已经已经关闭时都可以继续执行，前者为了增加元素，后者为了回收资源
    space_cond.wait(lock,
                    [&]() -> bool { return queue.size() < capacity || done; });

    if (done) {
      throw std::runtime_error("Queue is closed!");
    }

    queue.push(num);
    lock.unlock();

    data_cond.notify_one();
  };

  auto pop = [&](int& num) -> bool {
    std::unique_lock<std::mutex> lock(mtx, std::defer_lock);
    lock.lock();

    data_cond.wait(lock, [&]() -> bool { return done || !queue.empty(); });

    if (done && queue.empty()) {
      lock.unlock();
      return false;
    }

    num = queue.front();
    queue.pop();

    lock.unlock();
    space_cond.notify_one();
    return true;
  };

  std::jthread producer([&]() -> void {
    std::random_device randie;
    std::mt19937_64 gen(randie());
    std::uniform_int_distribution<int> dist(1, 200);

    try {
      for(int i = 0; i < 20; i++){
        int value = dist(gen);
        logger.info("produce num is: {}", value);
        push(value);

        std::this_thread::sleep_for(std::chrono::microseconds(dist(gen)));
      }
    } catch (const std::exception& e) {
      logger.info("produce error, the error is: {}", e.what());
    }

    logger.info("produce success");
  });

  std::jthread consumer([&]() -> void{
    std::this_thread::sleep_for(std::chrono::seconds(2));

    int value = 0;
    while (pop(value)) {
      logger.info("consumer value is: {}", value);

      std::this_thread::sleep_for(std::chrono::microseconds(200));
    }
    logger.info("consume success");
  });

  producer.join();
  done = true;
  data_cond.notify_all();
  space_cond.notify_all();  
}

void MutexDeadLock::lockGuardCall() noexcept {
  // 简单的锁包装器，源码也是非常简单，利用RAII实现的自动加锁和解锁，没有任何实际的函数
    
  std::mutex mtx;
  std::atomic<std::uint32_t> num = 0;

  auto func = [](std::atomic<std::uint32_t>& num_, std::mutex& mtx_) -> void {
    while (num_.load() < 100) {
      std::lock_guard<std::mutex> lock{mtx_};

      if (num_.load() < 100) {
        num_.fetch_add(1);
      } else {
        break;
      }

      logger.info("My thread id is: {}, and the num is: {}", globalVariable.hasher(std::this_thread::get_id()), num_.load());
    }
  };

  std::jthread thr_1{func, std::ref(num), std::ref(mtx)};
  std::jthread thr_2{func, std::ref(num), std::ref(mtx)};
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
      std::unique_lock<std::mutex> lock1{mtx1_, std::defer_lock};
      std::unique_lock<std::mutex> lock2{mtx2_, std::defer_lock};

      // std::lock()底层实现了一个避免死锁的算法，即全部加锁，全部解锁，如果遍历的过程中加锁失败，则回退前面的锁进行解锁
      std::lock(lock1, lock2);  
      
      if (num1_.load() < 100) {
        num1_.fetch_add(1);
      } else {
        break;
      }

      if (num2_.load() < 100) {
        num2_.fetch_add(1);
      } else {
        break;
      }

      logger.info( "My thread id is: {}, and the num1 is: {}, and the num2 is: {}", globalVariable.hasher(std::this_thread::get_id()), num1_.load(), num2_.load());
    }
  };

  std::jthread thr_1{func, std::ref(num1), std::ref(num2), std::ref(mtx1), std::ref(mtx2)};
  std::jthread thr_2{func, std::ref(num1), std::ref(num2), std::ref(mtx1), std::ref(mtx2)};
}

void MutexDeadLock::scopedLockCall() noexcept {
  /*
    scoped_lock: https://en.cppreference.com/w/cpp/thread/scoped_lock
    底层如果传入一把锁和lock_guard类似，多把锁则直接调用std::lock()
  */
  std::mutex mtx;
  std::atomic_uint32_t num = 0;

  auto func = [](std::atomic_uint32_t& num_, std::mutex& mtx_) -> void {
    while (num_.load() < 100) {
      std::scoped_lock<std::mutex> lock{mtx_};

      if (num_.load() < 100) {
        num_.fetch_add(1);
      } else {
        break;
      }

      logger.info( "My thread id is: {}, and the num is: {}", globalVariable.hasher(std::this_thread::get_id()), num_.load());
    }
  };

  std::jthread thr_1{func, std::ref(num), std::ref(mtx)};
  std::jthread thr_2{func, std::ref(num), std::ref(mtx)};
}

void MutexDeadLock::sharedLockCall() noexcept {
  /*
    shared_lock: https://en.cppreference.com/w/cpp/thread/shared_lock
    读锁使用shared_lock，写锁使用unique_lock
  */
  std::shared_mutex shmtx;
  std::unordered_map<std::string, std::string> entries;

  auto write = [&](const std::string& key, const std::string& value) -> void {
    std::unique_lock<std::shared_mutex> lock{shmtx};

    entries[key] = value;
    logger.info("write shared_entries, [{} is {}] ", key, value);
  };

  auto read = [&](const std::string& key) -> void {
    std::shared_lock<std::shared_mutex> lock{shmtx};

    logger.info("read shared_entries, [{} is {}]", key, entries[key]);
  };

  entries["key1"] = "value1";
  entries["key2"] = "value2";
  entries["key3"] = "value3";

  std::jthread thr_1{read, "key1"};
  std::jthread thr_2{read, "key2"};
  std::jthread thr_3{write, "key4", "value4"};
}

}  // namespace core