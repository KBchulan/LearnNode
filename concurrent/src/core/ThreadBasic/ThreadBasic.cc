#include "ThreadBasic.hpp"

#include <chrono>
#include <thread>

namespace core {

void ThreadBasic::basic() noexcept {
  // 启动一个线程，传入一个函数的地址和参数，线程启动后会直接执行这个回调
  std::thread thr_1(thread_work_1, "hello, thread");
  
  std::this_thread::sleep_for(std::chrono::seconds(1));
  // thr_1.join();
}

void ThreadBasic::thread_work_1(std::string &&str) noexcept {
  std::puts(str.c_str());
}

}  // namespace core