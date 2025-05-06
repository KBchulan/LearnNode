/******************************************************************************
 *
 * @file       UnlockQueue.hpp
 * @brief      利用原子操作和内存序实现无锁队列
 *
 * @author     KBchulan
 * @date       2025/05/06
 * @history
 ******************************************************************************/

#ifndef UNLOCKQQUEUE_HPP
#define UNLOCKQQUEUE_HPP

#include "middleware/Logger.hpp"
#include <core/CoreExport.hpp>
#include <cstddef>
#include <global/Singleton.hpp>
#include <memory>
#include <mutex>
#include <utility>

namespace core {

template <typename Type, std::size_t Capacity>
class CircleQueueLock final : public global::Singleton<CircleQueueLock<Type, Capacity>>, private std::allocator<Type> {
public:
  CircleQueueLock()
      : max_size_(Capacity + 1), data_(std::allocator<Type>::allocate(max_size_)) {}

  CircleQueueLock(const CircleQueueLock &) = delete;
  CircleQueueLock &operator=(const CircleQueueLock &) = delete;
  CircleQueueLock(CircleQueueLock &&) = delete;
  CircleQueueLock &operator=(CircleQueueLock &&) = delete;

  ~CircleQueueLock() {
    std::scoped_lock<std::mutex> lock{mtx};
    while (head_ != tail_) {
      std::destroy_at(data_ + head_);
      head_ = (head_ + 1) % max_size_;
    }
    std::allocator<Type>::deallocate(data_, max_size_);
  }

  template <typename... Args> bool emplace(Args &&...args) {
    std::scoped_lock<std::mutex> lock{mtx};

    if ((tail_ + 1) % max_size_ == head_) {
      logger.warning("the queue full");
      return false;
    }
    std::construct_at(data_ + tail_, std::forward<Args>(args)...);
    tail_ = (tail_ + 1) % max_size_;
    return true;
  }

  bool push(const Type &val) { return emplace(val); }

  bool push(Type &&val) { return emplace(std::move(val)); }

  bool pop(Type &val) {
    std::scoped_lock<std::mutex> lock{mtx};
    if (head_ == tail_) {
      logger.warning("the queue empty");
      return false;
    }
    val = std::move(data_[head_]);
    std::destroy_at(data_ + head_);
    head_ = (head_ + 1) % max_size_;
    return true;
  }

private:
  std::size_t max_size_;
  std::size_t head_{};
  std::size_t tail_{};
  Type* data_;
  std::mutex mtx;
};

class TestQueue final : public global::Singleton<TestQueue> {
public:
  static void test1();

  static void test2();
};

} // namespace core

#define testQueue core::TestQueue::getInstance()

#endif // !UNLOCKQQUEUE_HPP