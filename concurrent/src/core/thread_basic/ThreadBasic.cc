#include "ThreadBasic.hpp"

#include <array>
#include <memory>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <exception>
#include <functional>
#include <string>
#include <string_view>
#include <thread>
#include <middleware/Logger.hpp>

#include <utils/guard/ThreadGuard.hpp>

namespace core {

void ThreadBasic::enterFunc() noexcept {
  // 启动一个线程，传入一个函数的地址和参数，线程启动后会直接执行这个回调
  std::thread thr_1(thread_work_1, "hello, thread");

  // 一个线程的joinable()默认是true，因此对线程的处理必须显示的join或者detach，否则析构会走terminate，也就是断言的方式强制终止程序并抛出异常
  // 对应cppreference: https://en.cppreference.com/w/cpp/error/terminate
  // 调用join后，会等待子线程执行完后才继续执行主进程，也就是27行输出会被延迟打印
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
  // 下面的oops，我们的主进程只会休眠1000ms，因此子进程并没有完全执行完就会退出，综上，在使用detach时，如果使用局部变量或指针，需要考虑资源的生命周期
  oops();
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // 那如何解决这个问题，1.局部变量用shared_ptr包装，这样在存在计数的情况下就不会被回收 2.改为join() 3.拷贝传递


  // 异常处理，当我们的主线程出现异常时，不应该直接抛出异常通知主进程回收资源，而是应该阻塞等待各个子线程任务结束后再回收资源，即在catch模块先join后throw
  catch_exception();


  // 使用ThreadGuard来管理线程(简单的RAII)
  auto_guard();


  // 当然, c++20后我们可以使用jthread来替代我们这个简单的ThreadGuard实现
  jthreadDemo();


  // 这里演示一个线程参数传递的潜在问题，我们启动一个线程，期望传递一个string，但是实际传入std::array<Tp = char, N>.data()数据，事实上底层存在一个机制，
  // 是将我们传入的参数储存起来，在实际运行中再进行类型转换，因此这会是一种不安全的行为，毕竟如果依赖的原数据被回收后，thread底层维持一个引用是无法在运行的时候转换成功的
  /*
   explicit _Invoker(_Args&&... __args) : _Tuple(std::forward<_Args>(__args)...) { }
  */
  danger_oops(4);

  // 因此比较安全的做法就是显示进行类型转化，直接存入转换后的数据，其实此时传入的会是一个副本，这样就比较安全
  safe_oops(4);


  // 引用传递，当我们存在一个线程的回调参数是一个引用类型的时候，我们需要通过使用std::ref/std::cref将传入的参数显示的转换为引用类型
  // 原因是args会在构造里走一次decay，这时引用->非引用，自然无法匹配，需要加入ref生成reference_wrapper对象才可以保证传入的参数的引用属性
  std::uint32_t refNum = 200;
  ref_oops(refNum);


  // 回调也可以是类的成员函数，静态函数和非静态都有例子
  class_oops();


  // 如果是一些unique类型的话，需要使用std::move来转移所有权
  unique_oops();
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

void ThreadBasic::jthreadDemo() noexcept {
  std::jthread joinThread([](std::string_view &&str) -> void
  {
    logger.info("this is jthread(c++20), and the print str is: {}", str);
  }, "hello, jthread");
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

void ThreadBasic::danger_oops(std::uint32_t params) noexcept {
  std::array<char, 1024> buffer;
  sprintf(buffer.data(), "%i", params);
  std::thread dangerThread([](std::string str) -> void {
    str = str + "hhh";
    logger.info(str);
  }, buffer.data());

  dangerThread.detach();
}

void ThreadBasic::safe_oops(std::uint32_t params) noexcept {
  std::array<char, 1024> buffer;
  sprintf(buffer.data(), "%i", params);
  std::thread dangerThread([](std::string str) -> void {
    str = str + "hhh";
    logger.info(str);
  }, static_cast<std::string>(buffer.data()));

  dangerThread.detach();
}

void ThreadBasic::ref_oops(std::uint32_t& params) noexcept {
  logger.info("Begin this refThread, the params is: {}", params);
  std::thread refThread([](std::uint32_t& num) -> void { num++; }, std::ref(params));
  refThread.join();
  logger.info("After this refThread, the params is: {}", params);
}

void ThreadBasic::class_oops() noexcept {
  test ttt;
  int num = 5;
  std::thread classThread(&test::ppp, &ttt, std::ref(num));
  std::thread __static_classThread(&test::print);
  classThread.join();
  __static_classThread.join();
}

void ThreadBasic::unique_oops() noexcept {
  std::unique_ptr<int> ptr = std::make_unique<int>(10);
  std::thread uniqueThread([ptr = std::move(ptr)]() -> void {
    logger.info("the ptr is: {}", *ptr);
  });
  uniqueThread.join();
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

void ThreadBasic::test::ppp(int &num) noexcept {
  logger.info("num: {}, the num_ is: {}", num, num_);
}

void ThreadBasic::test::print() noexcept {
  logger.info("i am a func in class");
}

}  // namespace core