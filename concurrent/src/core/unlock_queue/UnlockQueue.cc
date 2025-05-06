#include "UnlockQueue.hpp"

#include <middleware/Logger.hpp>

namespace core {

void TestQueue::test1() {
  CircleQueueLock<int, 50> queue;

  for (int i = 0; i < 50; ++i) {
    if (queue.push(i)) {
      logger.info("成功入队: {}", i);
    } else {
      logger.error("入队失败: {}", i);
    }
  }

  if (!queue.push(100)) {
    logger.error("队列已满，无法入队: {}", 100);
  }

  int value;
  for (int i = 0; i < 50; ++i) {
    if (queue.pop(value)) {
      logger.info("成功出队: {}", value);
    } else {
      logger.error("出队失败");
    }
  }

  // 测试队列已空
  if (!queue.pop(value)) {
    logger.error("队列已空，无法出队");
  }
}

} // namespace core