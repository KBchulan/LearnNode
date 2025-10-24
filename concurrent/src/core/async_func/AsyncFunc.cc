#include "AsyncFunc.hpp"

#include <exception>
#include <functional>
#include <future>
#include <middleware/Logger.hpp>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>

namespace core {

void AsyncFunc::enterFunc() noexcept {
  // futureCall();

  // packageCall();

  // promiseCall();

  // exceptionCall();

  // destructCall();

  sharedfutureCall();
}

void AsyncFunc::futureCall() noexcept {
  using namespace std::chrono_literals;

  // 异步启动的任务
  auto func = []() -> std::string {
    std::this_thread::sleep_for(5s);
    return "return value";
  };

  /*
    这里我们可以看一下std::async的第一个参数：launch __policy，表示一个启动策略
      - std::launch::async：异步的执行，即调用到std::async时会直接启动一个后台线程进行该任务的处理
      - std::launch::defered：延迟执行，即在调用std::futher::get() 或 std::futher::wait()才会执行(此处是在主线程进行执行的)
      默认行为是这两者同时选取，由编译器决定选择什么策略
  */
  std::future<std::string> res = std::async(std::launch::async, func);
  logger.info("Main thread is doint something...");

  /*
    std::future::get()：阻塞主线程直到获取future的返回值，且只能调用一次，会消费底层的共享状态
    std::future::wait()：只是阻塞主线程直到异步任务结束，不获取返回值，但可以调用多次，不会消费底层的共享状态
    std::future::wait_for()：等待异步任务一段时间，有ready(任务已完成)，timeout(未完成)，defered(任务没开始，策略选择有关)
    std::future::wait_until()：等待到某个时间点，其余和上一个相同
    std::future::valid()：表示底层的共享状态是否有效，如果被get了或者被move了都会置为false
    std::future::share()：移交共享状态所有权到一个shared_future对象中
  */
  logger.info("the res: {}", res.get());
  // if(res.wait_for(3s) == std::future_status::ready) { }
}

void AsyncFunc::packageCall() noexcept {
  using namespace std::chrono_literals;

  auto func = []() -> int {
    std::this_thread::sleep_for(2s);
    logger.info("I worked 2s");
    return 5555;
  };

  std::packaged_task<int()> task{func};
  std::future<int> res = task.get_future();

  // 这里必须采用移动的方式把任务包装器传递给新的线程
  std::thread work{std::move(task)};
  work.detach();

  auto status = res.wait_for(3s);
  if (status == std::future_status::ready) {
    logger.info("The return value is: {}", res.get());
  } else if (status == std::future_status::timeout) {
    logger.info("The work failed complete in 3s");
  }
}

void AsyncFunc::promiseCall() noexcept {
  using namespace std::chrono_literals;

  auto func = [](std::promise<int> &pro) -> void {
    std::this_thread::sleep_for(3s);
    pro.set_value(10);

    std::this_thread::sleep_for(3s);
    logger.info("Child thread exit");
  };

  std::promise<int> pro;
  std::future<int> res = pro.get_future();

  std::jthread work(func, std::ref(pro));
  logger.info("the promise value is: {}", res.get());
}

void AsyncFunc::exceptionCall() noexcept {
  using namespace std::chrono_literals;
  // 此处子线程会抛出异常
  auto func = [](std::promise<void> &pro) -> void {
    try {
      auto thred_id = std::hash<std::thread::id>{}(std::this_thread::get_id());
      throw std::runtime_error(
          "A errno occured in son thread, the thread_id is " +
          std::to_string(thred_id));
    } catch (...) {
      pro.set_exception(std::current_exception());
    }
  };

  std::promise<void> pro;
  std::future<void> res = pro.get_future();

  try {
    std::jthread work(func, std::ref(pro));

    // 子线程的异常被存储到这个future里面了，如果获取这个future的内容则会导致异常重新抛出
    auto status = res.wait_for(2s);
    if (status == std::future_status::ready) {
      res.get();  // 这里会再次抛出异常
    }
  } catch (const std::exception &exc) {
    logger.error("{}", exc.what());
  }
}

void AsyncFunc::destructCall() noexcept {
  using namespace std::chrono_literals;

  auto func = [](std::promise<int> &pro) -> void {
    std::this_thread::sleep_for(2s);
    pro.set_value(20);
  };

  std::jthread thr;
  std::future<int> res;
  {
    std::promise<int> pro;
    res = pro.get_future();
    thr = std::jthread(func, std::ref(pro));
  }
  // 此处promise的东西会被释放，但是由于这些的底层都是共同维护一个共享数据，future还存在引用，因此此处不会报错
  logger.info("the return value is: {}", res.get());
}

void AsyncFunc::sharedfutureCall() noexcept {
  std::mutex mtx;

  using namespace std::chrono_literals;
  auto task = [](std::promise<int> &pro) -> void {
    std::this_thread::sleep_for(1s);
    pro.set_value(15);
  };

  auto print = [&mtx](const std::shared_future<int> &sha) -> void {
    try {
      auto status = sha.wait_for(2s);

      if (status == std::future_status::ready) {
        std::scoped_lock<std::mutex> guard{mtx};
        logger.info("the value is: {}", sha.get());
      } else if (status == std::future_status::timeout) {
        logger.warning("the task failed executable");
      } else {
        logger.error("the task has been set to defered");
      }

    } catch (const std::exception &exc) {
      logger.error("the error msg is: {}", exc.what());
    }
  };

  std::promise<int> pro_;
  std::shared_future<int> shaFut = pro_.get_future().share();

  std::jthread work{task, std::ref(pro_)};
  std::jthread consu1{print, shaFut};
  std::jthread consu2{print, shaFut};
}

} // namespace core