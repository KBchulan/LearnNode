/******************************************************************************
 *
 * @file       ThreadPool.hpp
 * @brief      一个线程池的设计
 *
 * @author     KBchulan
 * @date       2025/04/23
 * @history
 ******************************************************************************/

#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace core {

class ThreadPool {
public:
  ThreadPool(const ThreadPool &) = delete;
  ThreadPool &operator=(const ThreadPool &) = delete;

  ThreadPool(ThreadPool &&) = delete;
  ThreadPool &operator=(const ThreadPool &&) = delete;

  static ThreadPool &getInstance() {
    static ThreadPool instance_;
    return instance_;
  }

  ~ThreadPool() { stop(); }

  template <typename Func, typename... Args>
  auto commit(Func&& func, Args&&... args) -> std::future<decltype(std::forward<Func>(func)(std::forward<Args>(args)...))> {

    using ReturnType = decltype(std::forward<Func>(func)(std::forward<Args>(args)...));

		if (stop_.load(std::memory_order_acquire)) {
			return std::future<ReturnType>{};
    }

    auto task = std::make_shared<std::packaged_task<ReturnType()>>(
        std::bind(std::forward<Func>(func), std::forward<Args>(args)...));

    std::future<ReturnType> ret = task->get_future();
    {
      std::lock_guard<std::mutex> guard{mtx_};
      tasks_.emplace([task]() -> void { (*task)(); });
    }

    cond_.notify_one();
    return ret;
  }

  [[nodiscard]] int availableThread() const {
    return static_cast<int>(available_num_);
  }

protected:
  ThreadPool(unsigned long nums = std::thread::hardware_concurrency()) {
    stop_.store(false, std::memory_order_release);

    if (nums < 1) {
      available_num_.store(1, std::memory_order_release);
    } else {
      available_num_.store(static_cast<std::uint8_t>(nums),
                           std::memory_order_release);
    }

    start();
  }

  void start() {
    volatile auto active = available_num_.load(std::memory_order_acquire);
    for (std::size_t i = 0; i < active; ++i) {
      threads_.emplace_back([this]() -> void {
        while (!stop_.load(std::memory_order_acquire)) {

          mtx_.lock();
          std::unique_lock guard{mtx_, std::adopt_lock};
          cond_.wait(guard, [this]() -> bool {
            return stop_.load(std::memory_order_acquire) || !tasks_.empty();
          });

          if (tasks_.empty()) {
            return;
          }

          std::packaged_task<void()> task = std::move(tasks_.front());
          tasks_.pop();
          guard.unlock();

          available_num_.fetch_sub(1, std::memory_order_relaxed);
          task();
          available_num_.fetch_add(1, std::memory_order_release);
        }
      });
    }
  }

  void stop() {
    stop_.store(true, std::memory_order_release);
    cond_.notify_all();
  }

private:
  std::mutex mtx_;
  std::condition_variable cond_;

  std::atomic_bool stop_;
  std::atomic_uint8_t available_num_;

  std::vector<std::jthread> threads_;
  std::queue<std::packaged_task<void()>> tasks_;
};

} // namespace core

#define threadPool core::ThreadPool::getInstance()

#endif // !THREAD_POOL_HPP
