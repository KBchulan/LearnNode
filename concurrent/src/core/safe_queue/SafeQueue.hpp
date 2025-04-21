/******************************************************************************
 *
 * @file       SafeQueue.hpp
 * @brief      使用条件变量实现线程安全的队列
 *
 * @author     KBchulan
 * @date       2025/04/21
 * @history
 ******************************************************************************/

#ifndef SAFEQUEUE_HPP
#define SAFEQUEUE_HPP

#include <condition_variable>
#include <core/CoreExport.hpp>
#include <global/Singleton.hpp>

#include <memory>
#include <mutex>
#include <queue>

namespace core {

class CORE_EXPORT SafeQueue final : public global::Singleton<SafeQueue> {
public:
  static void enterFunc() noexcept;

private:
  static void printEach() noexcept;

  static void testQueue() noexcept;

  template <typename T> class ThreadSafeQueue {
  public:
    ThreadSafeQueue() = default;
    ~ThreadSafeQueue() = default;

    ThreadSafeQueue(const ThreadSafeQueue &other) {
      std::scoped_lock<std::mutex, std::mutex> lock{this->mtx, other.mtx};
      this->queue_ = other.queue_;
    }

    void push(T new_value) {
      std::unique_lock<std::mutex> lock{mtx};
      queue_.push(new_value);
      cond.notify_one();
    }

    void wait_and_pop(T &value) {
      std::unique_lock<std::mutex> lock{mtx};
      cond.wait(lock, [this]() -> bool { return !queue_.empty(); });
      value = queue_.front();
      queue_.pop();
    }

    std::shared_ptr<T> wait_and_pop() {
      std::unique_lock<std::mutex> lock{mtx};
      cond.wait(lock, [this]() -> bool { return !queue_.empty(); });
      std::shared_ptr<T> res{std::make_shared<T>(queue_.front())};
      queue_.pop();
      return res;
    }

    bool try_pop(T &value) {
      std::unique_lock<std::mutex> lock{mtx};
      if (queue_.empty()) {
        return false;
      }
      value = queue_.front();
      queue_.pop();
      return true;
    }

    std::shared_ptr<T> try_pop() {
      std::unique_lock<std::mutex> lock{mtx};
      if (queue_.empty()) {
        return std::make_shared<T>();
      }
      std::shared_ptr<T> res{std::make_shared<T>(queue_.front())};
      queue_.pop();
      return res;
    }

    bool empty() const {
      std::lock_guard<std::mutex> lock(mtx);
      return queue_.empty();
    }

  private:
    std::condition_variable_any cond;
    mutable std::mutex mtx;
    std::queue<T> queue_;
  };
};

} // namespace core

#define safeQueue core::SafeQueue::getInstance()

#endif // SAFEQUEUE_HPP
