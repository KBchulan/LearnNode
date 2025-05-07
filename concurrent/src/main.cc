#include <cmath>
#include <core/actor_csp/ActorCsp.hpp>
#include <core/async_func/AsyncFunc.hpp>
#include <core/atomic_memory/AtomicMemory.hpp>
#include <core/mutex_deadlock/MutexDeadLock.hpp>
#include <core/parallel_compute/ParallelCompute.hpp>
#include <core/safe_queue/SafeQueue.hpp>
#include <core/thread_basic/ThreadBasic.hpp>
#include <core/thread_manage/ThreadManage.hpp>
#include <core/thread_pool/ThreadPool.hpp>
#include <core/unlock_queue/UnlockQueue.hpp>

int main() {
  // 1.线程基础
  // core::ThreadBasic::enterFunc();

  // 2.线程管控
  // core::ThreadManage::enterFunc();

  // 3.互斥和死锁
  // core::MutexDeadLock::enterFunc();

  // 4.条件变量实现线程安全队列
  // core::SafeQueue::enterFunc();

  // 5.异步三剑客
  // core::AsyncFunc::enterFunc();

  // 6.线程池
  // extern void test_thread_pool();
  // test_thread_pool();

  // 7.并行计算
  // core::ParallelCompute::enterFunc();

  // 8.两种并发编程的设计模式
  // core::ActorCsp::enterFunc();

  // 9.原子操作和内存序
  // core::AtomicMemory::enterFunc();

  // 10.内存序实现无锁队列
  core::TestQueue::test3();
}
