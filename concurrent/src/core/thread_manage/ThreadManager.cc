#include <cstddef>
#include <thread>
#include <utility>
#include <vector>

#include <global/Global.hpp>
#include <middleware/Logger.hpp>

#include "ThreadManage.hpp"

namespace core {

void ThreadManage::enterFunc() noexcept {
  // 这里介绍一个管控的危险使用例子，我们已经给了修改方案，这里可以看出，线程对象本身是唯一的，
  // 需要注意对线程对象的join(否则会走析构和移动赋值的terminate)，因此涉及move一定要注意
  dangerousUse();


  // 这里简单介绍一个RVO和NRVO优化，如下面这个函数按照传统理解，会在函数的栈帧中构造一个局部对象，然后按照移动/拷贝的方式
  // 返回给调用者，但是实际上在c++17以后会强制采用RVO优化，即直接在调用者的内存空间构造这个对象，而不是采用移动
  auto returnThread = rvoAndnrvoUse();
  returnThread.join();


  // 我们在上一届略微介绍了jthread，这里在展开说一下
  jthreadUse();


  // 使用vector存储我们的线程对象
  vectorUse();


  // 这里介绍一个并行计算的
  accUse();


  // 线程识别
  threadInfo();
}

void ThreadManage::dangerousUse() noexcept {
  std::thread thr_1([]() -> void { logger.info("Before move, i am thread1"); });
  std::thread thr_2 = std::move(thr_1);

  // thr_1此时为空，可以管控一个线程对象，此时joinable() = true
  thr_1 = std::thread([]() -> void { logger.info("After move, i am thread1"); });

  // 此时再把thr_2移动赋值给thr_1，就会引发崩溃(release下)，即走terminate逻辑
  // 源码非常简单，在移动赋值里有一个if (joinable()) std::__terminate();
  logger.info("i am normal");
  thr_1.join();  // after线程
  thr_1 = std::move(thr_2);
  logger.info("i am normal");

  thr_1.join();  // before线程
}

std::thread ThreadManage::rvoAndnrvoUse() noexcept {
  return std::thread([]() -> void { logger.info("i am a return thread obj(rvo)"); });
}

void ThreadManage::jthreadUse() noexcept {
  // 内部在析构或者赋值的位置给了自动化管理join的方式，而不是生硬的terminate
  std::jthread joinThread([]() -> void {
    logger.info("i am jthread callback");
  });

  // 除此之外，内部额外引入了stop_source，允许协作时取消线程
  joinThread.request_stop();
}

void ThreadManage::vectorUse() noexcept {
  std::vector<std::thread> threads;
  auto printParams = [](std::size_t num) -> void {
    logger.info("current num is: {}", num);
  };

  threads.reserve(3);
  for (std::size_t i = 0; i < 3; i++) {
    // 如果选择push_back：构造出来线程对象，随后std::move进入vector内存区域
    // 而emplace_back会直接在vector内存空间进行构造，可以节约一次move开销
    // 但是这个push_back的行为在RVO优化下，在一些情况可以达到emplace_back的效率
    threads.emplace_back(printParams, i);
  }

  for (auto &thread : threads) {
    thread.join();
  }
}

void ThreadManage::threadInfo() noexcept {
  logger.info("current thread id is: {}", globalVariable.hasher(std::this_thread::get_id()));
}

void ThreadManage::accUse() noexcept {
  std::vector<int> nums(10000, 1);
  auto result = parrelAcc<std::vector<int>::iterator, int>(nums.begin(), nums.end(), 0);
  logger.info("the result is: {}", result);
}

}  // namespace core