#include <core/async_func/AsyncFunc.hpp>
#include <core/mutex_deadlock/MutexDeadLock.hpp>
#include <core/parallel_compute/ParallelCompute.hpp>
#include <core/safe_queue/SafeQueue.hpp>
#include <core/thread_basic/ThreadBasic.hpp>
#include <core/thread_manage/ThreadManage.hpp>
#include <core/thread_pool/ThreadPool.hpp>
#include <core/actor_csp/ActorCsp.hpp>
#include <core/atomic_memory/AtomicMemory.hpp>

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
  // core::AsyncFunc::enterFunc();

  // 线程池
  // extern void test_thread_pool();
  // test_thread_pool();

  // 并行计算
  // core::ParallelCompute::enterFunc();

  // 两种并发编程的设计模式
  // core::ActorCsp::enterFunc();

  // 原子和内存序的初步开始
  core::AtomicMemory::enterFunc();
}
