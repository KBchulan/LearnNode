#include "AtomicMemory.hpp"

#include <atomic>
#include <cassert>
#include <middleware/Logger.hpp>
#include <thread>

namespace core {

void AtomicMemory::enterFunc() noexcept {
  // 原子操作的介绍
  // atomicFunc();
  // relaxedFunc();
  // seqConsFunc();
  acqrelFunc();
}

void AtomicMemory::atomicFunc() noexcept {
  /*
    - 改动序列：c++中对一个对象的所有写操作形成一个改动序列，其中第一个写操作就是对象的初始化。每次运行这个改动序列可以不一样，但是单次运行时所有线程必须形成一致的改动序列
      - 只要某线程看到过某个对象，则该线程的后续读操作必须获得相对新近的值，并且，该线程就同一对象的后续写操作，必然出现在改动序列后方。
      - 如果某线程先向一个对象写数据，过后再读取它，那么必须读取前面写的值。
      - 若在改动序列中，上述读写操作之间还有别的写操作，则必须读取最后写的值。
      - 在程序内部，对于同一个对象，全部线程都必须形成相同的改动序列，并且在所有对象上都要求如此.
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
      - Sequencial consistent ordering：实现同步, 且保证全局顺序一致(single total order)的模型. 是一致性最强的模型, 也是默认的顺序模型
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
      只能保证操作的原子性，(x，y的修改不会被中断，它一定是true/false，而不会是修改被中断产生的乱码)
      无法实现同步，(线程1操作的x对线程2不是立即可见的)
      也无法保证全局顺序一致 (线程1是x -> y，线程2完全可以是y -> x)
  */
  std::atomic_bool bxval_{false};
  std::atomic_bool byval_{false};

  auto write_x_then_y = [&]() -> void {
    // release版本下编译器进行指令重排，y操作先于x
    bxval_.store(true, std::memory_order_relaxed);
    byval_.store(true, std::memory_order_relaxed);
  };

  auto read_y_then_x = [&]() -> void {
    while (!byval_.load(std::memory_order_relaxed)) {
      logger.info("y load false");
    }

    assert(bxval_.load(std::memory_order_relaxed));
  };

  std::jthread thr_1{write_x_then_y};
  std::jthread thr_2{read_y_then_x};
}

void AtomicMemory::seqConsFunc() noexcept {
  /*
    最强的顺序模型，可以保证全局顺序一致，底层是这样的：一个core在写入的数据到memory之前都是写失效的，其他线程是不能读取的，因此
    会造成一些额外的开销，如一个bank需要多走一层从cache到memory的路径，因此存在一定性能丢失
    seq_cst保证全局顺序一致且与程序执行顺序相同，如在线程1中x->y->z，那么在线程2一定也是这个顺序
    其他内存模型接受编译器或者处理器指令重排，无法保证所有线程的执行顺序相同
  */
  std::atomic_bool bxval_{false};
  std::atomic_bool byval_{false};
  std::atomic_int izval_{0};

  auto write_x_then_y = [&]() -> void {
    bxval_.store(true, std::memory_order_seq_cst);
    byval_.store(true, std::memory_order_seq_cst);
  };

  auto read_y_then_x = [&]() -> void {
    while (!byval_.load(std::memory_order_seq_cst)) {
      logger.info("y load false");
    }

    if (bxval_.load(std::memory_order_seq_cst)) {
      izval_.fetch_add(1, std::memory_order_seq_cst);
    }
  };

  std::jthread thr_1{write_x_then_y};
  std::jthread thr_2{read_y_then_x};
  thr_1.join();
  thr_2.join();
  logger.info("the z value is: {}", izval_.load());
  // 这个一定不会触发，因为这个内存序可以保证同步和全局顺序一致，
  // 即线程1对x,y进行修改，那么线程2可见修改结果，且由于顺序一致，1修改了y，则x也一定被修改，也就是z一定fetch_add了，那么
  // 此情况一定是true;不会触发断言
  assert(izval_.load() != 0);
}

void AtomicMemory::acqrelFunc() noexcept {
  /*
    release 操作保证在此操作之前的所有内存写入对稍后执行的acquire操作的线程可见且有序
    acquire 操作保证在此操作之后的所有内存读取/写入(在本线程内)不会被重排到此操作之前

    acq-rel不会产生新的指令，只是影响编译器的优化，任何指令不会被编排到acquire前面，也不会编排到release后面
  */
  std::atomic_bool xval{false};
  std::atomic_bool yval{false};

  std::jthread thr_1{[&]() -> void {
    xval.store(true, std::memory_order_relaxed);
    yval.store(true, std::memory_order_release); // 此操作保证之前所有的修改(包括非原子的)对于acquire操作都是可见的
  }};

  // rel-acq形成的同步关系是一对一的，如果这个线程与thr_2形成同步关系，那么会导致断言被触发
  std::jthread danthr{[&yval]() -> void {
    yval.store(true, std::memory_order_release);
  }};

  std::jthread thr_2{[&]() -> void {
    while (!yval.load(std::memory_order_acquire)) {
      logger.info("the y load false");
    }
    assert(xval.load(std::memory_order_relaxed));
  }};
}

} // namespace core