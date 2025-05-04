#include "AtomicMemory.hpp"

#include <atomic>
#include <cassert>
#include <middleware/Logger.hpp>
#include <thread>

namespace core {

void AtomicMemory::enterFunc() noexcept {
  // 原子操作的介绍
  atomicFunc();
}

void AtomicMemory::atomicFunc() noexcept {
  /*
    - 改动序列：c++中对一个对象的所有写操作形成一个改动序列，其中第一个写操作就是对象的初始化。每次运行这个改动序列可以不一样，但是单次运行时所有线程必须形成一致的改动序列
      - 只要某线程看到过某个对象，则该线程的后续读操作必须获得相对新近的值，并且，该线程就同一对象的后续写操作，必然出现在改动序列后方。
      - 如果某线程先向一个对象写数据，过后再读取它，那么必须读取前面写的值。
      - 若在改动序列中，上述读写操作之间还有别的写操作，则必须读取最后写的值。
      - 在程序内部，对于同一个对象，全部线程都必须就其形成相同的改动序列，并且在所有对象上都要求如此.
      - 多个对象上的改动序列只是相对关系，线程之间不必达成一致，线程需要维护的是同一个对象的改动序列一致
  */

  // std::atomic<>::is_always_lock_free 是一个编译期常量，c++17及以后可以用这个内置函数判断是否是无锁结构，
  // 如果返回true，表明在当前平台上，对该类型的所有原子操作都能通过硬件指令直接实现，不需要使用互斥锁
  logger.info("the atomic<bool>: {}", std::atomic_bool::is_always_lock_free);

  // 接下来是内存序相关内容，https://gitbookcpp.llfc.club/sections/cpp/concurrent/concpp11.html
  /*
    共有六种(现为五种)内存序：
      - memory_order_relaxed：不保证顺序，只保证原子性
      - memory_order_acquire：读取操作，建立后续读取的屏障
      - memory_order_release：写入操作，建立之前写入的屏障
      - memory_order_acq_rel：组合 `acquire`和 `release`语义
      - memory_order_seq_cst：最严格的顺序，保证所有线程看到一致的操作顺序

    三种内存模型：
      - Sequencial consistent ordering：实现同步, 且保证全局顺序一致 (single total order) 的模型. 是一致性最强的模型, 也是默认的顺序模型
      - Acquire-release ordering. 实现一个变量的同步, 但不保证多个变量全局顺序一致的模型
      - Relaxed ordering. 不能实现同步, 只保证原子性的模型

    前置知识：
    1. 硬件组成：
        - cpu(store buffer, read buffer) --- Cache(三层缓存) --- Memory(内存) --- 外存
        且一般两块cpu core组成一个bank，共享一个Cache，所有cpu共享Memory，因此在任意时刻，所有的核都有可能向Cache或者Memory
        写入数据，那么同一时刻不同的bank中多个核同时对一个variable进行写入是不是就会导致数据不一致，那么如何避免呢，引入第二条

    2. MESI协议：是一种叫作写失效(Write Invalidate)的协议。在写失效协议里，只有一个 CPU 核心负责写入数据，其他的核心
       只是同步读取到这个写入。在这个 CPU 核心写入 cache 之后，它会去广播一个“失效”请求告诉所有其他的 CPU 核心。
          - M：代表已修改(Modified)：用来告诉其他cpu已经修改完成，其他cpu可以向cache中写入数据
          - E：代表独占(Exclusive)：表示数据只是加载到当前 CPU核 的storebuffer中，其他的 CPU 核并没有加载对应的数据到自己的 storebuffer 里
          - S：代表共享(Shared)：就是在多核中同时加载了同一份数据
          - I：代表已失效(Invalidated)：一个核开始写时会通知所有核心对这个变量的写失效，只能读

    3. 同步："A synchronizes-with B" 的意思就是A和B同步，简单来说如果多线程环境下，有一个线程先修改了变量m，我们将这个操作叫做A，
            之后有另一个线程读取变量m，我们将这个操作叫做B，那么B一定读取A修改m之后的最新值

    4. 先行："A happens-before B" 的意思是如果A操作先于B操作，那么A操作的结果对B操作可见(我们可以看出同步是先行的一种)
  */
}

void AtomicMemory::relaxedFunc() noexcept {
  /*
    宽松内存序(memory_order_relaxed)：
      - 作用于原子变量
      - 不具有synchronizes-with关系
      - 在同一个线程中,对于同一个原子变量具有happens-before关系, 不同的原子变量不具有happens-before关系，可以乱序执行
      - 多线程情况下不具有happens-before关系
  */
  std::atomic_bool bxval_{false};
  std::atomic_bool byval_{false};
  std::atomic_int izval_{0};

  auto write_x_then_y = [&]() -> void {
    bxval_.store(true, std::memory_order_relaxed);
    byval_.store(true, std::memory_order_relaxed);
  };

  auto read_y_then_x = [&]() -> void {
    while (!byval_.load(std::memory_order_relaxed)) {
      logger.info("y load false");
    }

    if (bxval_.load(std::memory_order_relaxed)) {
      izval_.fetch_add(1, std::memory_order_relaxed);
    }
  };

  std::jthread thr_1{write_x_then_y};
  std::jthread thr_2{read_y_then_x};
  thr_1.join();
  thr_2.join();
  logger.info("the z value is: {}", izval_.load());
  assert(izval_.load() == 1); // 可能会被触发，分析详见上面的url
}

} // namespace core