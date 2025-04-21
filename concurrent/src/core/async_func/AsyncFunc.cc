#include "AsyncFunc.hpp"

#include <future>
#include <middleware/Logger.hpp>
#include <string>
#include <thread>

namespace core {

void AsyncFunc::enterFunc() noexcept {
  // 一个简单的异步任务
  asyncTask();
}

void AsyncFunc::asyncTask() noexcept {
  using namespace std::chrono_literals;

  auto func = []() -> std::string {
    std::this_thread::sleep_for(5s);
    return "return value";
  };

  std::future<std::string> res = std::async(std::launch::async, func);
  logger.info("Main thread is doint something...");
  logger.info("the res: {}", res.get());
}

} // namespace core