#include "UnlockQueue.hpp"

#include <middleware/Logger.hpp>
#include <mutex>
#include <thread>

namespace core {

void TestQueue::test1() {
  CircleQueueLock<int, 500> queue;
  std::mutex mtx;

  std::jthread thr_1{[&]() -> void {
    for (int i = 0; i < 250; ++i) {
      if (queue.push(i)) {
        std::scoped_lock<std::mutex> lock{mtx};
        logger.info("成功入队: {}", i);
      } else {
        std::scoped_lock<std::mutex> lock{mtx};
        logger.error("入队失败: {}", i);
      }
    }
  }};

  std::jthread thr_2{[&]() -> void {
    for (int i = 1000; i < 1250; ++i) {
      if (queue.push(i)) {
        std::scoped_lock<std::mutex> lock{mtx};
        logger.info("成功入队: {}", i);
      } else {
        std::scoped_lock<std::mutex> lock{mtx};
        logger.error("入队失败: {}", i);
      }
    }
  }};
  thr_1.join();
  thr_2.join();

  if (!queue.push(100)) {
    std::scoped_lock<std::mutex> lock{mtx};
    logger.error("队列已满，无法入队: {}", 100);
  }

  int value;
  std::jthread thr_3{[&]() -> void {
    for (int i = 0; i < 500; ++i) {
      if (queue.pop(value)) {
        std::scoped_lock<std::mutex> lock{mtx};
        logger.info("成功出队: {}", value);
      } else {
        std::scoped_lock<std::mutex> lock{mtx};
        logger.error("出队失败");
      }
    }
  }};

  thr_3.join();

  // 测试队列已空
  if (!queue.pop(value)) {
    logger.error("队列已空，无法出队");
  }
}

void TestQueue::test2() {
  CircleQueueUnlock<int, 500> queue;
  std::mutex mtx;

  std::jthread thr_1{[&]() -> void {
    for (int i = 0; i < 250; ++i) {
      if (queue.push(i)) {
        std::scoped_lock<std::mutex> lock{mtx};
        logger.info("成功入队: {}", i);
      } else {
        std::scoped_lock<std::mutex> lock{mtx};
        logger.error("入队失败: {}", i);
      }
    }
  }};

  std::jthread thr_2{[&]() -> void {
    for (int i = 1000; i < 1250; ++i) {
      if (queue.push(i)) {
        std::scoped_lock<std::mutex> lock{mtx};
        logger.info("成功入队: {}", i);
      } else {
        std::scoped_lock<std::mutex> lock{mtx};
        logger.error("入队失败: {}", i);
      }
    }
  }};
  thr_1.join();
  thr_2.join();

  if (!queue.push(100)) {
    std::scoped_lock<std::mutex> lock{mtx};
    logger.error("队列已满，无法入队: {}", 100);
  }

  int value;
  std::jthread thr_3{[&]() -> void {
    for (int i = 0; i < 500; ++i) {
      if (queue.pop(value)) {
        std::scoped_lock<std::mutex> lock{mtx};
        logger.info("成功出队: {}", value);
      } else {
        std::scoped_lock<std::mutex> lock{mtx};
        logger.error("出队失败");
      }
    }
  }};

  thr_3.join();

  // 测试队列已空
  if (!queue.pop(value)) {
    logger.error("队列已空，无法出队");
  }
}

void TestQueue::test3() {
  CircleQueueLockFree<int, 500> queue;
  std::mutex mtx;

  std::jthread thr_1{[&]() -> void {
    for (int i = 0; i < 250; ++i) {
      if (queue.push(i)) {
        std::scoped_lock<std::mutex> lock{mtx};
        logger.info("成功入队: {}", i);
      } else {
        std::scoped_lock<std::mutex> lock{mtx};
        logger.error("入队失败: {}", i);
      }
    }
  }};

  std::jthread thr_2{[&]() -> void {
    for (int i = 1000; i < 1250; ++i) {
      if (queue.push(i)) {
        std::scoped_lock<std::mutex> lock{mtx};
        logger.info("成功入队: {}", i);
      } else {
        std::scoped_lock<std::mutex> lock{mtx};
        logger.error("入队失败: {}", i);
      }
    }
  }};

  int value;
  std::jthread thr_3{[&]() -> void {
    for (int i = 0; i < 500; ++i) {
      if (queue.pop(value)) {
        std::scoped_lock<std::mutex> lock{mtx};
        logger.info("成功出队: {}", value);
      } else {
        std::scoped_lock<std::mutex> lock{mtx};
        logger.error("出队失败");
      }
    }
  }};
}

} // namespace core