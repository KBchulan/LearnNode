#include "SafeQueue.hpp"

#include <atomic>
#include <condition_variable>
#include <middleware/Logger.hpp>
#include <mutex>
#include <random>
#include <thread>

namespace core {

void SafeQueue::enterFunc() noexcept {
  // 这里实现一个交替打印
  // printEach();

  // 测试安全队列
  testQueue();
}

void SafeQueue::printEach() noexcept {
  /*
    条件变量：当调用到wait时，如果返回false，他会在一个原子操作中解锁并投递到等待队列，此时完全休眠，不占用任何cpu，
    当调用到notify时，则会进入就绪态，尝试获取锁，成功就开始执行
  */
  std::atomic_int16_t shared_num{0};

  std::mutex mtx;

  std::condition_variable_any cd1;
  std::condition_variable_any cd2;

  std::jthread thr_1([&]() -> void {
    while (true) {
      std::unique_lock<std::mutex> lock{mtx, std::defer_lock};
      lock.lock();

      cd2.wait(lock, [&]() -> bool { return shared_num.load() == 1; });
      shared_num.fetch_sub(1, std::memory_order_seq_cst);
      logger.info("the shared value is: {}", shared_num.load());

      lock.unlock();
      cd1.notify_one();
    }
  });

  std::jthread thr_2([&]() -> void {
    while (true) {
      std::unique_lock<std::mutex> lock{mtx, std::defer_lock};
      lock.lock();

      cd1.wait(lock, [&]() -> bool { return shared_num.load() == 0; });
      shared_num.fetch_add(1, std::memory_order_seq_cst);
      logger.info("the shared value is: {}", shared_num.load());

      lock.unlock();
      cd2.notify_one();
    }
  });
}

void SafeQueue::testQueue() noexcept {
  using namespace std::chrono_literals;

  std::mutex mtx;
  auto queue = ThreadSafeQueue<int>{};

  std::jthread producer([&]() -> void {
    std::random_device rand;
    std::mt19937_64 mt64{rand()};
    std::uniform_int_distribution<int> uniform(1, 100);

    while (true) {
      auto num = uniform(mt64);
      queue.push(num);
      {
        std::lock_guard<std::mutex> guard(mtx);
        logger.info("producer num is: {}", num);
      }

      std::this_thread::sleep_for(200ms);
    }
  });

  std::jthread consumer1([&]() -> void {
    auto num = 0;
    while (true) {
      queue.wait_and_pop(num);
      {
        std::lock_guard<std::mutex> guard(mtx);
        logger.info("consumer1 get the num is: {}", num);
      }

      std::this_thread::sleep_for(500ms);
    }
  });

  std::jthread consumer2([&]() -> void {
    while (true) {
      auto res = queue.try_pop();
      {
        std::lock_guard<std::mutex> guard(mtx);
        logger.info("consumer1 get the num is: {}", *res);
      }

      std::this_thread::sleep_for(500ms);
    }
  });
}

} // namespace core