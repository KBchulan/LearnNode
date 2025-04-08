#include "ThreadBasic.hpp"

#include <chrono>
#include <cstdint>
#include <exception>
#include <middleware/Logger.hpp>
#include <string>
#include <string_view>
#include <thread>

#include <utils/guard/ThreadGuard.hpp>

namespace core {

void ThreadBasic::basic() noexcept {
  // 启动一个线程，传入一个函数的地址和参数，线程启动后会直接执行这个回调
  std::thread thr_1(thread_work_1, "hello, thread");

  // 一个线程的joinable()默认是true，因此对线程的处理必须显示的join或者detach，否则会走terminate，也就是断言的方式强制终止程序并抛出异常
  // 对应cppreference: https://en.cppreference.com/w/cpp/error/terminate
  // 调用join后，会等待子线程执行完后才继续执行主进程，也就是下面一行输出会被延迟打印
  thr_1.join();
  logger.info("after the thread1");


  // std::thread thr_2(background_tast());
  // 当我们想传入一个仿函数对象的话，他不会按照我们的期望区定义一个thread类型对象，而是会把他认为是一个函数的声明，这显然不符合我们的预期

  // 因此我们可以多加一层()：
  std::thread thr_2((background_tast()), "222");
  thr_2.join();

  // 或者采用c++11 的列表初始化来告诉编译器，这是在初始化一个变量(推荐)：
  std::thread thr_3 = std::thread{background_tast(), "333"};
  thr_3.join();


  // 当然我们也可以选择传入一个lambda表达式
  std::thread thr_4 = std::thread(
      [](std::string_view&& str) -> void {
        logger.info("thr_4 str is: {}", str);
      },
      "hello");
  thr_4.join();


  // 上述四个线程都是使用join，但是thread源码我们会发现线程也可以使用detach()分离线程，但是需要注意的是，如果分离的子线程依赖于一些局部变量
  // 如主进程的local或者子线程自己的，可能出现一个问题：主进程退出回收资源，导致子线程访问的这个局部变量被回收，那就可能出现一些未定义行为，比如
  // 下面的oops，我们的主进程只会休眠1000ms，因此子进程并没有完全执行完就会退出，综上，在使用detach时，如果使用局部变量或指针，需要考虑资源的回收
  oops();
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // 那如何解决这个问题，1.局部变量用shared_ptr包装，这样在存在计数的情况下就不会被回收 2.改为join() 3.拷贝传递


  // 异常处理，当我们的主线程出现异常时，不应该直接抛出异常通知主进程回收资源，而是应该阻塞等待各个子线程任务结束后再回收资源，即在catch模块先join后throw
  catch_exception();


  // 使用ThreadGuard来管理线程(简单的RAII)
  auto_guard();


  // 当然, c++20后我们可以使用jthread来替代我们这个简单的ThreadGuard实现
  jthreadDemo();
}

}  // namespace core

namespace core {

void ThreadBasic::thread_work_1(std::string&& str) noexcept {
  logger.info(str);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

void ThreadBasic::oops() noexcept {
  std::uint32_t local_variable = 0;
  std::thread detachThread{func(local_variable)};
  detachThread.detach();
}

void ThreadBasic::catch_exception() {
  std::uint32_t local_variable = 0;
  std::thread sonThread{func(local_variable)};

  try {
    logger.error("i am main thread, and i happen a error");
  } catch (const std::exception &e) {
    sonThread.join();
    throw;
  }

  sonThread.join();
}

void ThreadBasic::auto_guard() noexcept {
  std::uint32_t local_variable = 0;
  std::thread guardThread{ func(local_variable) };
  utils::ThreadGuard recu(guardThread);
  logger.info("the guard func has finished");
}

void ThreadBasic::jthreadDemo() noexcept{
  std::jthread joinThread([](std::string_view &&str) -> void
  {
    logger.info("this is jthread(c++20), and the print str is: {}", str);
  }, "hello, jthread");
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

void ThreadBasic::background_tast::operator()(std::string&& str = "") {
  logger.info(str.length() != 0 ? str : "this is a background_task");
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

ThreadBasic::func::func(std::uint32_t& num) : num_(num) {}

void ThreadBasic::func::operator()() noexcept {
  for (std::uint32_t i = 0; i < 3; i++) {
    num_ = i;
    logger.info("the local variable now is: {}", num_);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
}

}  // namespace core