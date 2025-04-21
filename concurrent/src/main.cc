#include <core/safe_queue/SafeQueue.hpp>
#include <core/mutex_deadlock/MutexDeadLock.hpp>
#include <core/thread_basic/ThreadBasic.hpp>
#include <core/thread_manage/ThreadManage.hpp>
#include <core/async_func/AsyncFunc.hpp>

int main() {
  // 线程基础
  // core::ThreadBasic::enterFunc();

  // 线程管控
  // core::ThreadManage::enterFunc();

  // 互斥和死锁
  // core::MutexDeadLock::enterFunc();

  // 条件变量实现线程安全队列
  // core::SafeQueue::enterFunc();

  // 异步三剑客
  core::AsyncFunc::enterFunc();
}
