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
#include <atomic>
#include <core/CoreExport.hpp>
#include <cstddef>
#include <global/Singleton.hpp>
#include <memory>
#include <mutex>
#include <new>
#include <utility>

#ifndef __cpp_lib_hardware_interference_size
#define CPP_LIB_HARDWARE_INTERFERENCE_SIZE_DEFAULT 64
namespace std {
    static constexpr size_t hardware_destructive_interference_size = CPP_LIB_HARDWARE_INTERFERENCE_SIZE_DEFAULT;
}
#endif

namespace core {

template <typename Type, std::size_t Capacity>
class CircleQueueLock final : private std::allocator<Type> {
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
      logger.error("the queue full");
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
      logger.error("the queue empty");
      return false;
    }
    val = std::move(data_[head_]);
    std::destroy_at(data_ + head_);
    head_ = (head_ + 1) % max_size_;
    return true;
  }

private:
  std::size_t max_size_;
  std::size_t head_{0};
  std::size_t tail_{0};
  Type* data_;
  std::mutex mtx;
};

// 此处的无锁指的是不是用互斥锁类似，但是依然通过原子变量实现了锁
template <typename Type, size_t Capicity>
class CircleQueueUnlock final : private std::allocator<Type> {
public:
  explicit CircleQueueUnlock()
      : max_size_(Capicity + 1),
        data_(std::allocator<Type>::allocate(max_size_)) {}

  CircleQueueUnlock(const CircleQueueUnlock &) = delete;
  CircleQueueUnlock &operator=(const CircleQueueUnlock &) = delete;
  CircleQueueUnlock(CircleQueueUnlock &&) = delete;
  CircleQueueUnlock &operator=(CircleQueueUnlock &&) = delete;

  ~CircleQueueUnlock() {
    while (head_ != tail_) {
      std::destroy_at(data_ + head_);
      head_ = (head_ + 1) % max_size_;
    }
    std::allocator<Type>::deallocate(data_, max_size_);
  }

  template <typename... Args> bool emplace(Args &&...args) {
    bool expected = false;
    while (!flag_.compare_exchange_weak(
        expected, true, std::memory_order_acquire, std::memory_order_relaxed)) {
      expected = false;
    }

    if ((tail_ + 1) % max_size_ == head_) {
      logger.error("the queue has full");
      flag_.store(false, std::memory_order_release);
      return false;
    }

    std::construct_at(data_ + tail_, std::forward<Args>(args)...);
    tail_ = (tail_ + 1) % max_size_;
    flag_.store(false, std::memory_order_release);
    return true;
  }

  bool push(const Type &val) { return emplace(val); }

  bool push(Type &&val) { return emplace(std::move(val)); }

  bool pop(Type &val) {
    bool expected = false;
    while (!flag_.compare_exchange_weak(
        expected, true, std::memory_order_acquire, std::memory_order_relaxed)) {
      expected = false;
    }

    if (head_ == tail_) {
      logger.error("the queue empty");
      flag_.store(false, std::memory_order_release);
      return false;
    }

    val = std::move(data_[head_]);
    std::destroy_at(data_ + head_);
    head_ = (head_ + 1) % max_size_;
    flag_.store(false, std::memory_order_release);
    return true;
  }

private:
  std::size_t max_size_;
  std::size_t head_{0};
  std::size_t tail_{0};
  Type* data_;
  std::atomic_bool flag_{false};  // 也可以用atomic_flag的test_and_set
};

/*
  上述队列存在一些问题：
    - head_ 的 pop 和 tail_ 的 push其实是可以同时进行的，只是说不能同时push或者pop，因此我们可以给头尾分别加入一个原子控制
    - 两个原子控制时会产生新的问题，如push和pop同一块内存时，无法保证Push和Pop的顺序性导致出现意外行为，因此需要引入额外的控制机制
  下面给出优化版本
*/

template <typename Type, size_t Capacity>
class CircleQueueLockFree final : private std::allocator<Type> {
public:
  static_assert(Capacity > 0, "Capacity must be greater than 0");
  static_assert(std::atomic<size_t>::is_always_lock_free, "std::atomic<size_t> must be lock-free.");

  explicit CircleQueueLockFree()
      : max_size_(Capacity + 1),
        data_(std::allocator<Type>::allocate(max_size_)) {
  }

  CircleQueueLockFree(const CircleQueueLockFree &) = delete;
  CircleQueueLockFree &operator=(const CircleQueueLockFree &) = delete;
  CircleQueueLockFree(CircleQueueLockFree &&) = delete;
  CircleQueueLockFree &operator=(CircleQueueLockFree &&) = delete;

  ~CircleQueueLockFree() {
    size_t current_h = head_.load(std::memory_order_relaxed);
    const size_t current_t = tail_.load(std::memory_order_relaxed);

    while (current_h != current_t) {
      std::destroy_at(data_ + current_h);
      current_h = (current_h + 1) % max_size_;
    }
    std::allocator<Type>::deallocate(data_, max_size_);
  }

  template <typename... Args> bool emplace(Args &&...args) {
    size_t current_tail = tail_.load(std::memory_order_acquire);

    while (true) {
      size_t head_snapshot = head_.load(std::memory_order_acquire);
      size_t next_tail = (current_tail + 1) % max_size_;

      if (next_tail == head_snapshot) {
        logger.error("the queue full");
        return false;
      }

      // 我们不能在这里直接构造，假设两个线程同时进入到这里，那么一个线程比较并更新，退出循环和构造，另外一个线程则会再走1 ~ 2个循环才能出去
      if (tail_.compare_exchange_weak(current_tail, next_tail, std::memory_order_acq_rel, std::memory_order_acquire)) {
        break;
      }
    }

    std::construct_at(data_ + current_tail, std::forward<Args>(args)...);
    return true;
  }

  bool push(const Type &val) { return emplace(val); }

  bool push(Type &&val) { return emplace(std::move(val)); }

  bool pop(Type &val) {
    size_t current_head = head_.load(std::memory_order_acquire);

    while (true) {
      size_t tail_snapshot = tail_.load(std::memory_order_acquire);

      if (current_head == tail_snapshot) {
        logger.error("the queue empty");
        return false;
      }

      size_t next_head = (current_head + 1) % max_size_;
      if (head_.compare_exchange_weak(current_head, next_head, std::memory_order_acq_rel, std::memory_order_acquire)) {
        break;
      }
    }

    val = std::move(data_[current_head]);
    std::destroy_at(data_ + current_head);
    return true;
  }

private:
  const std::size_t max_size_;

  alignas(std::hardware_destructive_interference_size) std::atomic<size_t> head_{0};
  alignas(std::hardware_destructive_interference_size) std::atomic<size_t> tail_{0};

  Type *data_;
};

class TestQueue final : public global::Singleton<TestQueue> {
public:
  static void test1();

  static void test2();

  static void test3();
};

} // namespace core

#define testQueue core::TestQueue::getInstance()

#endif // !UNLOCKQQUEUE_HPP