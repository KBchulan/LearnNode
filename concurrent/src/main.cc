#include <atomic>
#include <core/mutex_deadlock/MutexDeadLock.hpp>
#include <core/thread_basic/ThreadBasic.hpp>
#include <core/thread_manage/ThreadManage.hpp>
#include <iostream>

int main() {
  // 线程基础
  // core::ThreadBasic::enterFunc();

  // 线程管控
  // core::ThreadManage::enterFunc();

  // 互斥和死锁
  // core::MutexDeadLock::enterFunc();

  std::atomic_int num1 = 5;
  std::cout << num1.load() << '\n';
}
