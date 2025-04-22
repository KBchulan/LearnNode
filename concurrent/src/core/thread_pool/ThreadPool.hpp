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

  ThreadPool(const ThreadPool &&) = delete;
  ThreadPool &operator=(const ThreadPool &&) = delete;

  static ThreadPool &getInstance() {
    static ThreadPool instance_;
    return instance_;
  }

  template <typename Func, typename... Args>
  auto commit(Func &&func, Args &&...args)
      -> std::future<decltype(func(args...))> {
    using ReturnType = decltype(func(args...));

    if (stop_.load(std::memory_order_acquire)) {
      return std::future<ReturnType>{};
    }

    auto task = std::make_shared<std::packaged_task<ReturnType()>>(
        std::bind(std::forward<Func>(func), std::forward<Args>(args)...));

    std::future<ReturnType> ret = task->get_future();

    {
      std::scoped_lock<std::mutex> guard{mtx_};
      tasks_.emplace([task]() -> void { (*task)(); });
    }

    cond_.notify_one();
    return ret;
  }

  ~ThreadPool() { stop(); }

private:
  ThreadPool(unsigned int size = std::thread::hardware_concurrency()) {
    stop_.store(false, std::memory_order_release);

    if (size < 1) {
      thread_num_.store(1, std::memory_order_release);
    } else {
      thread_num_.store(size, std::memory_order_release);
    }

    start();
  }

  void start() {
    for (std::size_t i = 0; i < thread_num_.load(std::memory_order_acquire);
         i++) {
      pool_.emplace_back([this]() -> void {
        while (!this->stop_.load(std::memory_order_relaxed)) {
          std::packaged_task<void()> task;

          std::unique_lock<std::mutex> guard{mtx_};
          this->cond_.wait(guard, [this]() -> bool {
            return this->stop_.load(std::memory_order_acquire) ||
                   !this->tasks_.empty();
          });

          if (this->tasks_.empty()) {
            return;
          }

          task = std::move(this->tasks_.front());
          this->tasks_.pop();
          guard.unlock();

          this->thread_num_.fetch_sub(1, std::memory_order_release);
          task();
          this->thread_num_.fetch_add(1, std::memory_order_release);
        }
      });
    }
  }

  void stop() {
    stop_.store(true, std::memory_order_release);
    cond_.notify_all();
  }

  std::mutex mtx_;
  std::condition_variable cond_;

  std::atomic_bool stop_;
  std::atomic_uint32_t thread_num_;

  std::vector<std::jthread> pool_;
  std::queue<std::packaged_task<void()>> tasks_;
};

} // namespace core

#endif // !THREAD_POOL_HPP
