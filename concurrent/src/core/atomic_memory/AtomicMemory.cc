#include "AtomicMemory.hpp"

#include <atomic>
#include <middleware/Logger.hpp>

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

  // std::atomic<>::is_always_lock_free 是一个编译期常量，c++17及以后可以用这个内置函数判断是否是无锁结构，如果返回true，表明在当前平台上，对该类型的所有原子操作都能通过硬件指令直接实现，不需要使用互斥锁
  logger.info("the atomic<bool>: {}", std::atomic_bool::is_always_lock_free);

  // 接下来是内存序相关内容，https://gitbookcpp.llfc.club/sections/cpp/concurrent/concpp11.html
}


} // namespace core