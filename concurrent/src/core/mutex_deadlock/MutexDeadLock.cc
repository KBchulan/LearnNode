#include "MutexDeadLock.hpp"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <exception>
#include <functional>
#include <global/Global.hpp>
#include <middleware/Logger.hpp>
#include <mutex>
#include <queue>
#include <random>
#include <ratio>
#include <semaphore>
#include <shared_mutex>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <vector>

namespace core {

enum class WorkMode : std::uint8_t {
  ShortCriticalSection,
  MediumCriticalSection,
  LongCriticalSection
};

template <typename LockType>
double benchmark(int num_threads, WorkMode mode, int operations_per_thread) {
  LockType lock;
  int counter = 0;
  std::vector<std::thread> threads;
  std::atomic_bool start{false};

  auto thread_func = [&](int) {
    while (!start.load()) {}

    for (int i = 0; i < operations_per_thread; i++) {
      lock.lock();

      ++counter;

      switch (mode) {
        case WorkMode::ShortCriticalSection:
          // 比较短的临界区
          break;
        case WorkMode::MediumCriticalSection:
          for (int j = 0; j < 100; ++j) {
          }
          break;
        case WorkMode::LongCriticalSection:
          for (int j = 0; j < 1000; ++j) {
          }
          break;
      }

      lock.unlock();
    }
  };

  threads.reserve(static_cast<size_t>(num_threads));
  for (int i = 0; i < num_threads; ++i) {
    threads.emplace_back(thread_func, i);
  }

  auto start_time = std::chrono::system_clock::now();
  start.store(true);

  for (auto& thr : threads) {
    thr.join();
  }

  auto end_time = std::chrono::system_clock::now();
  std::chrono::duration<double, std::milli> elapsed = end_time - start_time;

  logger.info("the last counter is: {}(it should be {})", counter,
              operations_per_thread * num_threads);

  return elapsed.count();
}

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
    // conditionVariableCall();
    // spinlockCall();
    // semaphoreCall();
    // atomicCall();
    // deadLockCall();

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
    
    从本质上来看，互斥锁有这么一个流程，当前线程尝试获取锁，如果该锁已经被其他线程占有，则此线程则会被投递到内核空间的等待队列(通过调用futex)，
    然后锁解开时会调用通知，释放等待队列，随后重新回到用户态，因此，多个上下文的切换导致的开销是很大的，且线程会被阻塞，这是不好的
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

void MutexDeadLock::spinlockCall() noexcept {
  /*
    自旋锁：当线程尝试获取已被其他线程持有的锁时，它不会被挂起，而是在一个循环中不断检查锁是否可用，也就是一个忙等待的东西
  */

  // 普通的自旋锁
  class SpinLock {
   public:
    // test_and_set可以设置标志位为true，并返回之前的值，这样就实现了自旋
    void lock() { while (flag.test_and_set(std::memory_order_acquire)) {} }

    void unlock() { flag.clear(std::memory_order_release); }

    bool try_lock() { return !flag.test_and_set(std::memory_order_acquire); }

   private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
  };

  // 退避的自旋锁，防止自旋次数过多
  class BackoffSpinLock {
   public:
    void lock() {
      int backoff = 1;

      while (true) {
        if (!flag.exchange(true, std::memory_order_acquire)) {
          return;
        }

        // 退避策略
        for (int i = 0; i < backoff; ++i) {
          #if defined(__x86_64__) || defined(__i386__)
                    __builtin_ia32_pause();
          #elif defined(__arm__) || defined(__aarch64__)
                    __asm__ volatile("yield")
          #else
                    std::this_thread::yield();
          #endif
        }
        backoff = std::min(backoff * 2, 1024);
      }
    }

    void unlock() { flag.store(false, std::memory_order_release); }

    bool try_lock() { return !flag.exchange(true, std::memory_order_acquire); }

   private:
    std::atomic_bool flag{false};
  };

  constexpr int OPERATIONS = 100000;
  constexpr int REPEAT = 3;

  logger.info("测试不同类型锁在各种工作负载下的性能\n");
  logger.info("=======================================\n");

  for (int num_threads : {1, 2, 4, 8, 16}) {
    logger.info("\n线程数: {}", num_threads);
    logger.info("---------------------------------------\n");

    for (auto mode : {WorkMode::ShortCriticalSection, WorkMode::MediumCriticalSection, WorkMode::LongCriticalSection}) {
      std::string mode_name;
      switch (mode) {
        case WorkMode::ShortCriticalSection:
          mode_name = "短临界区";
          break;
        case WorkMode::MediumCriticalSection:
          mode_name = "中等临界区";
          break;
        case WorkMode::LongCriticalSection:
          mode_name = "长临界区";
          break;
      }

      logger.info("模式: {}", mode_name);

      std::vector<double> spin_times;
      std::vector<double> backoff_times;
      std::vector<double> mutex_times;

      for (int i = 0; i < REPEAT; ++i) {
        spin_times.push_back(benchmark<SpinLock>(num_threads, mode, OPERATIONS));
        backoff_times.push_back(benchmark<BackoffSpinLock>(num_threads, mode, OPERATIONS));
        mutex_times.push_back(benchmark<std::mutex>(num_threads, mode, OPERATIONS));
      }

      double spin_avg = std::accumulate(spin_times.begin(), spin_times.end(), 0.0) / REPEAT;
      double backoff_avg = std::accumulate(backoff_times.begin(), backoff_times.end(), 0.0) / REPEAT;
      double mutex_avg = std::accumulate(mutex_times.begin(), mutex_times.end(), 0.0) / REPEAT;

      logger.info("简单自旋锁: {}", spin_avg);
      logger.info("退避自旋锁: {}", backoff_avg);
      logger.info("互斥锁: {}", mutex_avg);
      logger.info("---------------------------------------\n");
    }
  }
}

void MutexDeadLock::semaphoreCall() noexcept {
  /*
    信号量的底层比较简单：维护一个内部计数器，表示可用资源的数量，如果为0则在acquire(获取)时可能阻塞，其余情况下可以递增

    二进制信号量：只有一个可用资源，此时功能类似于互斥量
    计数信号量：维护有限资源的访问

    Lock.md中有一个连接池的例子
  */
  std::binary_semaphore mainTothread{0};
  std::binary_semaphore sonToMain{0};

  auto workThread = [&]() -> void {
    // 阻塞在这里等待主线程增加
    mainTothread.acquire();

    logger.info("[Son] i am work thread ,i have got the signal");
    using namespace std::literals;
    std::this_thread::sleep_for(3s);

    sonToMain.release();
  };  

  std::jthread work{workThread};

  logger.info("[Main] i send a singal");
  mainTothread.release();

  sonToMain.acquire();
  logger.info("[Main] i got the signal");
}

void MutexDeadLock::atomicCall() noexcept {
  /*
    cppreference: https://en.cppreference.com/w/cpp/atomic/atomic
    原子操作是不可分割的操作单元，在操作过程中不会被中断，是无锁并发的基础，也是其他同步原语的底层实现
  
    基础的方法这里不介绍了，主要说一下简单的内存序: 
     - memory_order_relaxed：不保证顺序，只保证原子性
     - memory_order_acquire：读取操作，建立后续读取的屏障
     - memory_order_release：写入操作，建立之前写入的屏障
     - memory_order_acq_rel：组合 `acquire`和 `release`语义
     - memory_order_seq_cst：最严格的顺序，保证所有线程看到一致的操作顺序
  */

  std::atomic_uint32_t counter{0};
  logger.info("the counter is: {}", counter.load(std::memory_order_acquire));
  counter.fetch_add(1, std::memory_order_acq_rel);
  logger.info("the counter is: {}", counter.load(std::memory_order_acquire));
}

void MutexDeadLock::deadLockCall() noexcept {
  /*
    A和B线程都确保都对自己的锁进行加锁，然后期望获取对方的锁，就形成了循环引用

    如果需要在一个函数里加多个锁，我们可以选择std::lock和std::defer_lock或std::adopt_lock进行加锁，或者直接std::scoped_lock
  */
  std::mutex lockA;
  std::mutex lockB;

  std::binary_semaphore AtoB{0};
  std::binary_semaphore BtoA{0};

  auto threadA = [&]() -> void {
    std::scoped_lock<std::mutex> lock(lockA);

    // A被阻塞在这里，等待B解锁
    BtoA.acquire();
    logger.info("A: i acquire success");
    AtoB.release();

    lockB.lock();
    logger.info("A: i complete");
    lockB.unlock();
  };

  auto threadB = [&]() -> void {
    std::scoped_lock<std::mutex> lock(lockB);

    BtoA.release();
    AtoB.acquire();
    logger.info("B: i acquire success");

    lockA.lock();
    logger.info("B: i complete");
    lockA.unlock();
  };

  std::jthread thr_A{threadA};
  std::jthread thr_B{threadB};
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
      // 当然我们也可以不用上面的defer_lock，选择使用这个std::unique_lock<std::mutex> lock(mtx1_, std::adopt_lock);
      
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