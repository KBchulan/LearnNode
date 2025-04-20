# C++并发编程中的锁机制详解

## 目录

1. [互斥锁(Mutex)](#互斥锁)
2. [读写锁(Reader-Writer Lock)](#读写锁)
3. [递归锁(Recursive Mutex)](#递归锁)
4. [条件变量(Condition Variable)](#条件变量)
5. [自旋锁(Spin Lock)](#自旋锁)
6. [信号量(Semaphore)](#信号量)
7. [原子操作(Atomic Operations)](#原子操作)
8. [超时锁(Timed Mutex)](#超时锁)
9. [共享锁(Shared Lock)](#共享锁)
10. [乐观锁与悲观锁](#乐观锁与悲观锁)
11. [读写意向锁](#读写意向锁)
12. [死锁问题与解决方案](#死锁问题)

`<a id="互斥锁"></a>`

## 1. 互斥锁(Mutex)

### 1.1 基本概念

互斥锁(Mutual Exclusion)是最基本也是使用最广泛的同步原语，用于保护共享资源不会被多个线程同时访问。它的核心思想非常简单：在任何时刻，只允许一个线程持有锁并访问共享资源。当一个线程尝试获取已被其他线程持有的锁时，该线程将被阻塞，直到锁被释放。

### 1.2 底层实现原理

互斥锁在不同操作系统上有不同的实现，但基本原理类似：

1. **Linux POSIX实现**：在Linux中，互斥锁基于 `pthread_mutex_t`实现。底层通常使用以下机制：

   - **快速路径(Fast Path)**：使用原子操作如 `compare-and-swap`(CAS)尝试获取锁。
   - **慢速路径(Slow Path)**：如果快速尝试失败，线程将被挂起(通过系统调用如 `futex 投递到内核的等待队列`)，此时会进入内核态，内核空间维护一个等待队列。
   - **内核调度支持**：当锁释放时，通过系统调用如 `futex_wake`唤醒等待的线程。
2. **Windows实现**：在Windows中，互斥锁基于 `CRITICAL_SECTION`或 `HANDLE`实现：

   - 使用了用户空间的自旋等待和内核对象的组合。
   - 先尝试在用户空间获取锁，失败后再进入内核态等待。
3. **自旋-阻塞混合策略**：现代互斥锁实现通常采用自旋-阻塞混合策略：

   - 首先尝试自旋几次(在CPU上忙等)
   - 如果自旋失败，才将线程挂起(让出CPU)
   - 这种策略平衡了短时间锁定的性能和长时间等待的资源消耗。

### 1.3 C++标准库中的互斥锁

C++11及以后版本提供了四种基本互斥锁类型：

1. **`std::mutex`** - 基本互斥锁
2. **`std::recursive_mutex`** - 允许同一线程多次加锁的互斥锁
3. **`std::timed_mutex`** - 支持超时的互斥锁
4. **`std::recursive_timed_mutex`** - 支持超时的递归互斥锁

互斥锁提供两种基本操作：

- `lock()` - 获取锁，如果锁不可用则阻塞
- `unlock()` - 释放锁

还有一个非阻塞的获取锁方法：

- `try_lock()` - 尝试获取锁，立即返回，成功返回true，失败返回false

### 1.4 RAII锁包装器

C++标准库提供了几种便捷的RAII风格锁包装器，用于自动管理锁的生命周期：

1. **`std::lock_guard`** - 简单的RAII包装器，在构造时锁定互斥量，在析构时解锁
2. **`std::unique_lock`** - 更灵活的RAII包装器，支持延迟锁定、定时锁定、锁转移等
3. **`std::scoped_lock`** (C++17) - 可以同时锁定多个互斥量，避免死锁

### 1.5 性能考虑

互斥锁在不同场景下的性能考虑：

1. **锁的粒度**：

   - 细粒度锁(保护小范围数据)可以提高并发性，但增加锁管理开销
   - 粗粒度锁(保护大范围数据)减少锁开销，但限制并发性
   - 这里可以举一个例子，如哈希表的实现，我们可以选择整个表加一个锁(粗粒度)，也可以给每一个桶都加一个锁(细粒度锁)
2. **锁竞争**：

   - 高竞争场景下，频繁的上下文切换会显著降低性能
   - 可以考虑使用读写锁或无锁算法改善高竞争情况
3. **锁使用模式**：

   - 尽量减少锁定时间
   - 避免在持有锁时执行耗时操作(如I/O)

### 1.6 详细示例

下面是一个互斥锁保护共享资源的完整示例：

```cpp
#include <mutex>
#include <thread>
#include <iostream>
#include <vector>
#include <chrono>

class ThreadSafeCounter {
private:
    std::mutex mutex_;  // 保护计数器的互斥锁
    int value_ = 0;     // 共享资源

public:
    // 增加计数器的值
    void increment() {
        // 自动锁定和解锁
        std::lock_guard<std::mutex> lock(mutex_);
        ++value_;
        // 锁离开作用域时自动释放
    }

    // 使用unique_lock示范延迟锁定
    void increment_with_delay() {
        std::unique_lock<std::mutex> lock(mutex_, std::defer_lock); // 创建未锁定的锁

        // 做一些不需要锁的准备工作
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        // 稍后锁定
        lock.lock();
        ++value_;
        // 锁离开作用域时自动释放
    }

    // 获取当前值
    int get() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return value_;
    }
};

int main() {
    ThreadSafeCounter counter;
    std::vector<std::thread> threads;

    // 创建10个线程，每个线程增加计数器1000次
    for (int i = 0; i < 10; ++i) {
        threads.push_back(std::thread([&counter]() {
            for (int j = 0; j < 1000; ++j) {
                counter.increment();
            }
        }));
    }

    // 等待所有线程完成
    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "最终计数器值: " << counter.get() << std::endl;
    // 应该输出10000

    return 0;
}
```

### 1.7 互斥锁的缺点

互斥锁虽然使用广泛，但也有一些明显的缺点：

1. **性能开销**：获取和释放锁涉及系统调用和上下文切换，开销较大。
2. **阻塞性**：线程在等待锁时完全阻塞，无法做其他工作。
3. **死锁风险**：不当使用可能导致死锁（考虑std::lock可避免这个问题)。
4. **优先级反转**：低优先级线程持有高优先级线程需要的锁时，会导致优先级反转问题。
5. **无法区分读写操作**：对于读多写少的场景，互斥锁限制了并发读取的可能性。

### 1.8 适用场景

互斥锁最适合以下场景：

1. 保护需要读写操作的共享数据结构
2. 访问模式中，读写操作大致均衡
3. 锁持有时间较短
4. 临界区代码执行速度快
5. 线程竞争程度中等或较低

`<a id="读写锁"></a>`

## 2. 读写锁(Reader-Writer Lock)

### 2.1 基本概念

读写锁(也称为共享-独占锁)是一种比互斥锁更加灵活的同步机制，它基于以下观察：对共享资源的访问可以分为"读"和"写"两种操作。多个线程可以同时读取共享资源而不会导致问题，但写操作需要独占访问权限。

读写锁维护两种锁状态：

- **读锁(共享锁)**：多个线程可以同时持有读锁
- **写锁(独占锁)**：同一时刻只允许一个线程持有写锁，且此时不允许任何读锁存在

### 2.2 底层实现原理

读写锁的实现比互斥锁更复杂，需要跟踪多种状态：

1. **基本数据结构**：

   - 读者计数器：记录当前有多少线程持有读锁
   - 写等待标志：标识是否有写线程在等待
   - 写锁持有标志：标识当前是否有线程持有写锁
   - 内部互斥锁：保护读写锁的内部状态
2. **实现策略**：

   - **读优先策略**：优先满足读请求，可能导致写线程饥饿
   - **写优先策略**：优先满足写请求，避免读线程饥饿
   - **公平策略**：按请求顺序公平处理读写请求
3. **Linux POSIX实现**：

   - 基于 `pthread_rwlock_t`结构
   - 使用原子变量、条件变量和互斥锁的组合实现
4. **Windows实现**：

   - 基于SRW(Slim Reader/Writer)锁
   - 针对读多写少场景优化的轻量级同步对象

### 2.3 C++标准库中的读写锁

C++17引入了标准的读写锁实现：

1. **`std::shared_mutex`** - 完整的读写锁实现
2. **`std::shared_timed_mutex`**(C++14) - 支持超时的读写锁

读写锁提供以下操作：

对于写锁(独占访问)：

- `lock()` / `unlock()` - 获取/释放写锁
- `try_lock()` - 尝试获取写锁，立即返回

对于读锁(共享访问)：

- `lock_shared()` / `unlock_shared()` - 获取/释放读锁
- `try_lock_shared()` - 尝试获取读锁，立即返回

### 2.4 RAII锁包装器

C++标准库为读写锁提供了特殊的RAII包装器：

1. **`std::unique_lock`** - 用于独占(写)锁
2. **`std::shared_lock`**(C++14) - 用于共享(读)锁

这些包装器确保锁的正确获取和释放，遵循RAII原则。

### 2.5 详细示例

以下是一个详细的读写锁使用示例，展示了如何在多线程环境中保护共享数据：

```cpp
#include <shared_mutex>
#include <thread>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <map>

class ThreadSafeDirectory {
private:
    std::shared_mutex mutex_;  // 读写锁
    std::map<std::string, std::string> entries_;  // 共享的目录数据

public:
    // 写操作：添加或更新条目(需要独占锁)
    bool add_or_update_entry(const std::string& key, const std::string& value) {
        std::unique_lock<std::shared_mutex> lock(mutex_);  // 获取独占(写)锁

        // 模拟一个耗时的写操作
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        entries_[key] = value;
        std::cout << "更新条目: [" << key << "] = " << value << std::endl;
        return true;
    }

    // 读操作：查找条目(只需要共享锁)
    std::string find_entry(const std::string& key) const {
        std::shared_lock<std::shared_mutex> lock(mutex_);  // 获取共享(读)锁

        // 模拟一个耗时的读操作
        std::this_thread::sleep_for(std::chrono::milliseconds(20));

        auto it = entries_.find(key);
        if (it == entries_.end()) {
            return "未找到";
        }

        std::cout << "读取条目: [" << key << "] = " << it->second << std::endl;
        return it->second;
    }

    // 另一个读操作：获取所有的键
    std::vector<std::string> get_all_keys() const {
        std::shared_lock<std::shared_mutex> lock(mutex_);  // 获取共享(读)锁

        std::vector<std::string> keys;
        for (const auto& entry : entries_) {
            keys.push_back(entry.first);
        }

        return keys;
    }

    // 写操作：删除条目(需要独占锁)
    bool delete_entry(const std::string& key) {
        std::unique_lock<std::shared_mutex> lock(mutex_);  // 获取独占(写)锁

        auto it = entries_.find(key);
        if (it == entries_.end()) {
            return false;
        }

        entries_.erase(it);
        std::cout << "删除条目: [" << key << "]" << std::endl;
        return true;
    }
};

int main() {
    ThreadSafeDirectory directory;

    // 预先添加一些条目
    directory.add_or_update_entry("one", "第一个");
    directory.add_or_update_entry("two", "第二个");
    directory.add_or_update_entry("three", "第三个");

    std::vector<std::thread> readers;
    std::vector<std::thread> writers;

    // 创建5个读线程，每个读线程读取多次
    for (int i = 0; i < 5; ++i) {
        readers.push_back(std::thread([&directory, i]() {
            for (int j = 0; j < 3; ++j) {
                directory.find_entry("one");
                directory.find_entry("two");
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }));
    }

    // 创建2个写线程
    for (int i = 0; i < 2; ++i) {
        writers.push_back(std::thread([&directory, i]() {
            directory.add_or_update_entry("four", "第四个-" + std::to_string(i));
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            directory.add_or_update_entry("five", "第五个-" + std::to_string(i));
        }));
    }

    // 等待所有线程完成
    for (auto& t : readers) {
        t.join();
    }
    for (auto& t : writers) {
        t.join();
    }

    // 验证最终结果
    auto keys = directory.get_all_keys();
    std::cout << "最终目录包含 " << keys.size() << " 个条目" << std::endl;
    for (const auto& key : keys) {
        std::cout << key << ": " << directory.find_entry(key) << std::endl;
    }

    return 0;
}
```

### 2.6 性能考虑

读写锁的性能特点：

1. **适合读多写少场景**：在读操作远多于写操作的场景中，读写锁可以显著提高并发性能。
2. **写锁开销**：获取写锁通常比普通互斥锁开销更大，因为需要等待所有读操作完成。
3. **公平性与性能权衡**：

   - 读优先策略：提高读取吞吐量，但可能导致写线程饥饿
   - 写优先策略：确保写操作不会被长时间推迟，但可能降低读取并发性
   - 公平策略：均衡读写操作，但可能导致额外的上下文切换
4. **锁升级/降级**：

   - 锁升级(从读锁升级到写锁)通常不被直接支持，因为可能导致死锁
   - 锁降级(从写锁降级到读锁)通常是安全的，并且在某些实现中直接支持
5. **内存开销**：共享锁通常比普通互斥锁需要更多的内存来存储额外状态。

### 2.7 读写锁的缺点

1. **实现复杂度**：比普通互斥锁实现更复杂，可能有更多的边缘情况。
2. **写锁延迟**：在读操作频繁的场景下，写操作可能会经历较长的延迟。
3. **可伸缩性限制**：在极高并发的场景下，共享锁内部的同步机制可能成为瓶颈。

### 2.8 适用场景

读写锁最适合以下场景：

1. **读多写少的访问模式**：当读操作远多于写操作时，读写锁可以显著提高并行性。
2. **读操作耗时**：当读操作需要相对较长的时间时，允许并发读取可以提高整体吞吐量。
3. **写操作可以容忍一定延迟**：写操作可能需要等待所有读操作完成。
4. **需要数据一致性**：相比无锁算法，读写锁确保了数据的一致性。
5. **缓存、配置存储、数据库等系统**：这些系统通常具有读多写少的访问模式。

`<a id="递归锁"></a>`

## 3. 递归锁(Recursive Mutex)

### 3.1 基本概念

递归锁(也称为可重入锁)是一种特殊类型的互斥锁，它允许同一个线程多次获取该锁而不会导致死锁。普通互斥锁在同一线程尝试第二次获取锁时会导致死锁，而递归锁则会跟踪锁的拥有者和获取次数，只有当锁被完全释放(获取次数归零)时，其他线程才能获取该锁。

### 3.2 底层实现原理

递归锁相比普通互斥锁，需要额外维护两个信息：

1. **锁的持有者(线程ID)**：记录当前持有锁的线程标识符。
2. **递归计数器**：记录锁被同一线程获取的次数。

实现原理：

1. **获取锁时**：

   - 如果锁空闲，将持有者设为当前线程，计数器设为1
   - 如果锁被占用但持有者是当前线程，增加计数器
   - 如果锁被其他线程占用，阻塞等待
2. **释放锁时**：

   - 检查当前线程是否为锁持有者，不是则错误
   - 减少计数器
   - 如果计数器变为0，释放锁，供其他线程使用
3. **POSIX实现**：

   - 使用 `pthread_mutex_t`并设置 `PTHREAD_MUTEX_RECURSIVE`属性
   - 内部使用线程ID比较和计数器来实现递归特性
4. **Windows实现**：

   - 在 `CRITICAL_SECTION`中内置了递归支持
   - 使用线程ID和递归计数器实现

### 3.3 C++标准库中的递归锁

C++11引入了两种递归锁：

1. **`std::recursive_mutex`** - 基本递归互斥锁
2. **`std::recursive_timed_mutex`** - 支持超时的递归互斥锁

递归锁提供与普通互斥锁相同的接口：

- `lock()` / `unlock()` - 获取/释放锁
- `try_lock()` - 尝试获取锁，立即返回

对于递归锁，这些操作具有特殊的语义：

- 同一线程可以多次调用 `lock()`，需要对应调用相同次数的 `unlock()`
- 锁只有在所有 `lock()`调用都匹配了 `unlock()`调用后才会被释放

### 3.4 RAII锁包装器

递归锁可以使用与普通互斥锁相同的RAII包装器：

1. **`std::lock_guard<std::recursive_mutex>`** - 简单的RAII包装器
2. **`std::unique_lock<std::recursive_mutex>`** - 更灵活的RAII包装器

这些包装器在处理递归锁时，会正确处理锁的获取和释放，不需要特殊处理。

### 3.5 详细示例

以下示例展示了递归锁的使用场景，特别是在递归函数中：

```cpp
#include <recursive_mutex>
#include <thread>
#include <iostream>
#include <vector>

class RecursiveCounter {
private:
    std::recursive_mutex mutex_;  // 递归互斥锁
    int value_ = 0;               // 共享资源

public:
    // 递归实现的斐波那契数列计算
    // 使用递归锁保护共享状态
    int fibonacci(int n) {
        std::lock_guard<std::recursive_mutex> lock(mutex_);

        // 增加计数器(在锁保护下)
        ++value_;

        std::cout << "线程 " << std::this_thread::get_id()
                  << " 进入fibonacci(" << n << "), 计数: "
                  << value_ << std::endl;

        // 递归基础情况
        if (n <= 1) {
            // 离开函数时减少计数器(仍在锁保护下)
            --value_;
            return n;
        }

        // 递归调用 - 在持有锁的情况下重新获取相同的锁
        // 使用普通互斥锁这里会死锁!
        int result = fibonacci(n - 1) + fibonacci(n - 2);

        // 离开前减少计数器
        --value_;

        return result;
    }

    // 复杂的递归树遍历示例
    void process_tree_node(int depth, int branching, int node_id) {
        std::lock_guard<std::recursive_mutex> lock(mutex_);

        if (depth <= 0) return;

        // 处理当前节点(修改共享状态)
        ++value_;

        std::cout << "处理节点: 深度=" << depth
                  << ", ID=" << node_id
                  << ", 计数=" << value_ << std::endl;

        // 递归处理子节点(仍持有相同的锁)
        for (int i = 0; i < branching; ++i) {
            process_tree_node(depth - 1, branching, node_id * 10 + i);
        }

        // 完成处理后可能有其他操作
        --value_;
    }

    // 获取计数器的当前值
    int get_value() const {
        // 即使对于只读操作，也需要锁保护
        std::lock_guard<std::recursive_mutex> lock(mutex_);
        return value_;
    }
};

int main() {
    RecursiveCounter counter;
    std::vector<std::thread> threads;

    // 创建几个线程计算斐波那契数
    for (int i = 0; i < 3; ++i) {
        threads.push_back(std::thread([&counter, i]() {
            int result = counter.fibonacci(5 + i);
            std::cout << "线程 " << std::this_thread::get_id()
                     << " 计算结果: fib(" << (5 + i) << ") = "
                     << result << std::endl;
        }));
    }

    // 再创建几个线程来处理树节点
    for (int i = 0; i < 2; ++i) {
        threads.push_back(std::thread([&counter, i]() {
            counter.process_tree_node(3, 2, i);
            std::cout << "线程 " << std::this_thread::get_id()
                     << " 完成树处理" << std::endl;
        }));
    }

    // 等待所有线程完成
    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "最终计数器值: " << counter.get_value() << std::endl;
    // 应该为0，因为每次进入都+1，离开都-1

    return 0;
}
```

### 3.6 性能考虑

递归锁相比普通互斥锁有以下性能特性：

1. **额外开销**：递归锁需要额外跟踪所有者线程和计数器，因此通常比普通互斥锁慢一些。
2. **用例场景**：

   - 适合有递归调用结构的代码
   - 适合复杂对象的内部同步，其中方法可能相互调用
3. **内存占用**：递归锁通常比普通互斥锁需要更多的内存来存储额外状态。
4. **锁争用**：递归获取通常比第一次获取快，因为仅涉及计数器增加而不需要完整的锁获取过程。

### 3.7 递归锁的缺点

1. **性能开销**：比普通互斥锁更慢，因为需要检查线程身份和维护计数器。
2. **设计信号**：过度依赖递归锁可能暗示设计问题，如不必要的复杂锁层次结构。
3. **错误风险**：可能掩盖锁使用中的逻辑错误，这些错误在使用非递归锁时会导致明显的死锁。
4. **更难调试**：由于可以多次获取同一个锁，锁的持有模式可能变得更复杂，更难以跟踪。

### 3.8 适用场景

递归锁最适合以下场景：

1. **递归算法**：当递归函数需要在调用过程中保持对共享数据的锁定时。
2. **复杂对象内部同步**：当对象的方法需要调用其他同样需要锁保护的方法时。
3. **库API设计**：当需要确保库的用户不会因重复获取锁而导致死锁时。
4. **树结构或图结构遍历**：当需要在遍历过程中持续锁定数据结构时。
5. **需要支持可重入性的接口**：当函数可能被直接或间接地重复调用，且每次调用都需要锁保护时。

### 3.9 最佳实践

1. **有意识地使用**：只有在确实需要递归锁特性时才使用，否则使用普通互斥锁。
2. **考虑替代设计**：在使用递归锁前，考虑是否可以通过重构代码来避免需要递归获取锁。
3. **文档化**：清晰地文档说明使用递归锁的原因和预期的锁获取模式。
4. **保持简单**：尽量保持锁的获取和释放逻辑简单清晰，避免复杂的嵌套获取模式。
5. **限制作用域**：使用RAII包装器(如 `std::lock_guard`)限制锁的作用域，确保锁的正确释放。

`<a id="条件变量"></a>`

## 4. 条件变量(Condition Variable)

### 4.1 基本概念

条件变量是一种同步原语，它允许线程等待特定条件变为真。与互斥锁不同，条件变量的主要目的不是保护共享数据，而是在特定条件满足时通知等待中的线程，从而避免忙等待(busy-waiting)。条件变量总是与互斥锁配合使用。

条件变量主要有两个操作：

- **等待(wait)**：线程检查条件，如果条件不满足，则阻塞并等待通知
- **通知(notify)**：当条件可能变为真时，通知一个或多个等待的线程

### 4.2 底层实现原理

条件变量的实现要解决几个关键问题：

1. **原子性解锁和等待**：线程必须在一个原子操作中解锁互斥锁并进入等待状态，避免错过通知。
2. **等待者队列管理**：需要维护一个等待线程的队列，以便在条件满足时唤醒它们（这个也是内核空间的一个队列 ）。
3. **虚假唤醒处理**：由于操作系统调度和信号的不确定性，等待线程可能在没有实际通知的情况下被唤醒。

具体实现：

1. **POSIX实现**：

   - 基于 `pthread_cond_t`结构
   - 使用互斥锁和内核等待队列实现
   - `pthread_cond_wait`函数在等待时原子地释放互斥锁
   - `pthread_cond_signal`和 `pthread_cond_broadcast`函数通知等待线程
2. **Windows实现**：

   - 使用条件变量对象和CRITICAL_SECTION结合
   - 内部使用内核等待和信号机制
3. **内部机制**：

   - 等待线程被放入休眠状态，不消耗CPU资源
   - 通知操作将一个或多个等待线程移动到就绪队列
   - 被唤醒的线程在继续执行前重新获取互斥锁

### 4.3 C++标准库中的条件变量

C++11引入了两种条件变量类型：

1. **`std::condition_variable`** - 只能与 `std::mutex`一起使用
2. **`std::condition_variable_any`** - 可以与任何满足互斥锁概念的类型一起使用

条件变量提供以下主要操作：

- **`wait(lock, predicate)`** - 等待条件满足，期间释放锁
- **`notify_one()`** - 通知一个等待的线程
- **`notify_all()`** - 通知所有等待的线程
- **`wait_for(lock, duration, predicate)`** - 带超时的等待
- **`wait_until(lock, time_point, predicate)`** - 等待直到特定时间点

### 4.4 正确使用模式

条件变量有特定的使用模式，需要严格遵循以避免错误：

**等待线程模式**：

```cpp
std::unique_lock<std::mutex> lock(mutex);
// 使用while循环而非if语句检查条件，以处理虚假唤醒
while (!condition) {
    cv.wait(lock);  // 等待通知
}
// 条件满足，处理...
```

2. **通知线程模式**：

```cpp
{
    std::lock_guard<std::mutex> lock(mutex);
    // 修改共享状态...
    condition = true;
} // 在通知前释放锁
cv.notify_one(); // 或 cv.notify_all()
```

3. **带断言的等待**：

```cpp
cv.wait(lock, []{ return condition; });  // 等价于上面的while循环
```

### 4.5 详细示例

下面是一个使用条件变量实现线程安全队列的完整示例：

```cpp
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <iostream>
#include <vector>
#include <chrono>
#include <random>

template<typename T>
class ThreadSafeQueue {
private:
    std::queue<T> queue_;                  // 实际存储数据的队列
    mutable std::mutex mutex_;             // 保护队列的互斥锁
    std::condition_variable data_cond_;    // 条件变量，用于通知有新数据
    std::condition_variable space_cond_;   // 条件变量，用于通知队列不再满
    size_t capacity_;                      // 队列容量上限
    bool done_;                            // 标记队列是否关闭

public:
    explicit ThreadSafeQueue(size_t capacity = SIZE_MAX)
        : capacity_(capacity), done_(false) {}

    // 向队列添加元素
    void push(T data) {
        std::unique_lock<std::mutex> lock(mutex_);

        // 等待队列有空间
        space_cond_.wait(lock, [this]{
            return queue_.size() < capacity_ || done_;
        });

        // 如果队列已关闭，则抛出异常
        if (done_) {
            throw std::runtime_error("Queue is closed");
        }

        // 添加数据到队列
        queue_.push(std::move(data));

        // 解锁互斥锁(通过unique_lock的析构函数)并通知一个等待的消费者
        lock.unlock();
        data_cond_.notify_one();
    }

    // 从队列中弹出元素(阻塞版本)
    bool pop(T& value) {
        std::unique_lock<std::mutex> lock(mutex_);

        // 等待队列有数据或关闭信号
        data_cond_.wait(lock, [this]{
            return !queue_.empty() || done_;
        });

        // 如果队列为空且已关闭，返回失败
        if (queue_.empty()) {
            return false;
        }

        // 获取队首元素
        value = std::move(queue_.front());
        queue_.pop();

        // 解锁后通知生产者队列有新空间
        lock.unlock();
        space_cond_.notify_one();
        return true;
    }

    // 尝试从队列弹出元素(非阻塞版本)
    bool try_pop(T& value) {
        std::unique_lock<std::mutex> lock(mutex_, std::try_to_lock);

        // 如果无法立即获取锁，返回失败
        if (!lock || queue_.empty()) {
            return false;
        }

        // 获取队首元素
        value = std::move(queue_.front());
        queue_.pop();

        // 解锁后通知生产者队列有新空间
        lock.unlock();
        space_cond_.notify_one();
        return true;
    }

    // 等待指定时间尝试弹出元素
    template<typename Rep, typename Period>
    bool try_pop_for(T& value, const std::chrono::duration<Rep, Period>& timeout) {
        std::unique_lock<std::mutex> lock(mutex_);

        // 等待队列有数据，带超时
        bool success = data_cond_.wait_for(lock, timeout, [this]{
            return !queue_.empty() || done_;
        });

        // 超时或队列为空且已关闭
        if (!success || queue_.empty()) {
            return false;
        }

        // 获取队首元素
        value = std::move(queue_.front());
        queue_.pop();

        // 解锁后通知生产者队列有新空间
        lock.unlock();
        space_cond_.notify_one();
        return true;
    }

    // 检查队列是否为空
    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    // 获取队列大小
    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

    // 关闭队列，不再接受新数据
    void close() {
        std::lock_guard<std::mutex> lock(mutex_);
        done_ = true;
        data_cond_.notify_all();  // 通知所有等待的消费者
        space_cond_.notify_all(); // 通知所有等待的生产者
    }
};

// 使用示例
int main() {
    ThreadSafeQueue<int> queue(10); // 容量为10的队列

    // 生产者线程
    std::thread producer([&queue]() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(1, 100);

        try {
            for (int i = 0; i < 20; ++i) {
                int value = dist(gen);
                std::cout << "生产: " << value << std::endl;
                queue.push(value);

                // 随机休眠一段时间
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(dist(gen)));
            }
        } catch (const std::exception& e) {
            std::cout << "生产者异常: " << e.what() << std::endl;
        }

        std::cout << "生产者完成" << std::endl;
    });

    // 消费者线程
    std::thread consumer([&queue]() {
        // 延迟启动，让队列先积累一些元素
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        int value;
        while (queue.pop(value)) {
            std::cout << "消费: " << value << std::endl;

            // 模拟处理时间
            std::this_thread::sleep_for(
                std::chrono::milliseconds(200));
        }

        std::cout << "消费者完成" << std::endl;
    });

    // 另一个使用超时的消费者
    std::thread timed_consumer([&queue]() {
        int value;
        while (true) {
            bool got_item = queue.try_pop_for(
                value, std::chrono::milliseconds(300));

            if (got_item) {
                std::cout << "定时消费者取出: " << value << std::endl;
            } else if (queue.empty()) {
                // 如果超时且队列为空，可能是队列已关闭
                std::cout << "定时消费者超时且队列为空" << std::endl;
                break;
            }
        }

        std::cout << "定时消费者完成" << std::endl;
    });

    // 等待生产者完成
    producer.join();

    // 生产完成后关闭队列
    std::cout << "关闭队列" << std::endl;
    queue.close();

    // 等待消费者完成
    consumer.join();
    timed_consumer.join();

    return 0;
}
```

### 4.6 高级模式与变体

条件变量支持多种高级同步模式：

1. **单生产者-单消费者**：最简单的模式，一个线程产生数据，一个线程消费数据。
2. **多生产者-多消费者**：多个线程同时生产和消费数据，需要注意：

   - 使用 `notify_one()`可能导致唤醒错误的等待者
   - 使用 `notify_all()`可能导致惊群效应(thundering herd)
   - 复杂条件检查可能需要额外的同步机制
3. **带优先级的等待**：实现等待线程的优先级排序需要自定义队列管理。
4. **屏障模式**：所有线程到达特定点后才能继续执行：

```cpp
std::mutex mutex;
std::condition_variable cv;
int count = 0;
const int THREAD_COUNT = 10;

// 在每个线程中:
{
    std::unique_lock<std::mutex> lock(mutex);
    ++count;
    if (count == THREAD_COUNT) {
        cv.notify_all(); // 所有线程都到达了
    } else {
        cv.wait(lock, [&]{ return count == THREAD_COUNT; });
    }
}
// 同步点之后的代码...
```

### 4.7 性能考虑

条件变量的性能特点：

1. **等待效率**：相比忙等待(自旋等待)，条件变量允许线程完全休眠，不消耗CPU资源。
2. **唤醒开销**：唤醒等待线程涉及系统调用和上下文切换，有一定开销。
3. **虚假唤醒**：处理虚假唤醒需要重新检查条件，可能引入额外开销。
4. **惊群效应**：`notify_all()`会唤醒所有等待线程，但通常只有一个能成功处理条件，其余线程会再次等待，造成不必要的上下文切换。
5. **扩展性**：在高并发场景下，频繁的条件变量通知可能成为性能瓶颈。

### 4.8 条件变量的缺点

1. **复杂性**：正确使用条件变量需要遵循特定模式，容易出错。
2. **易误用**：常见错误包括：

   - 忘记在循环中检查条件
   - 在持有锁的状态下通知
   - 使用多个互斥锁导致死锁
3. **可调试性**：条件变量相关的问题通常难以调试，因为涉及复杂的线程交互。
4. **有限的保证**：条件变量只保证等待线程会被唤醒，但不保证何时被唤醒，可能导致响应延迟。

### 4.9 适用场景

条件变量最适合以下场景：

1. **生产者-消费者模式**：当一个或多个线程生产数据，另一些线程消费数据时。
2. **任务分派**：当主线程需要等待工作线程完成任务时。
3. **资源池管理**：当线程需要等待共享资源变为可用时。
4. **状态变化通知**：当线程需要等待特定状态变化时。
5. **替代轮询**：当需要避免周期性检查状态变化的开销时。

`<a id="自旋锁"></a>`

## 5. 自旋锁(Spin Lock)

### 5.1 基本概念

自旋锁是一种基于忙等待(busy-waiting)的锁机制，当线程尝试获取已被其他线程持有的锁时，它不会被挂起，而是在一个循环中不断检查锁是否可用。自旋锁的核心理念是：在短时间内等待锁释放比上下文切换更有效率。

自旋锁通常用于以下情形：

- 预期锁被持有的时间非常短
- 线程调度和上下文切换开销较大的环境
- 实时系统中，需要避免不可预测的调度延迟

### 5.2 底层实现原理

自旋锁的实现比互斥锁简单得多，主要基于原子操作：

**基本实现**：

* 使用一个原子变量(通常是布尔值或整数)表示锁状态
* 使用原子的"测试并设置"(Test-and-Set)或"比较并交换"(Compare-and-Swap, CAS)操作尝试获取锁
* 在无法获取锁时，线程持续尝试原子操作，直到成功

**优化策略**：

- **简单自旋**：不断尝试获取锁，不做任何其他操作
- **带退避的自旋**：在连续失败后增加尝试间隔，减少内存总线竞争
- **有限自旋**：限制自旋次数，超过后转为阻塞等待
- **暂停指令**：在循环中使用 `PAUSE`(x86)或 `YIELD`(ARM)指令，优化CPU性能

**硬件支持**：

- 现代CPU提供特殊指令支持原子操作
- 缓存一致性协议确保多核环境下锁状态对所有线程都可见

### 5.3 在C++中实现自旋锁

C++标准库不直接提供自旋锁，但可以使用 `std::atomic`轻松实现：

1. **使用 `std::atomic_flag`的简单实现**：

```cpp
class SpinLock {
private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT;

public:
    void lock() {
        while (flag.test_and_set(std::memory_order_acquire));
    }

    void unlock() {
        flag.clear(std::memory_order_release);
    }

    bool try_lock() {
        return !flag.test_and_set(std::memory_order_acquire);
    }
};
```

2. **带退避的实现**：

```cpp
class BackoffSpinLock {
private:
    std::atomic<bool> flag{false};

public:
    void lock() {
        int backoff = 1;
        while (true) {
            if (!flag.exchange(true, std::memory_order_acquire)) {
                return;
            }

            // 指数退避策略
            for (int i = 0; i < backoff; ++i) {
                // 使用编译器内置函数提示CPU我们在自旋
                #if defined(__x86_64__) || defined(__i386__)
                    __builtin_ia32_pause();
                #elif defined(__arm__) || defined(__aarch64__)
                    __asm__ volatile ("yield");
                #else
                    // 通用回退策略
                    std::this_thread::yield();
                #endif
            }

            // 增加退避时间，但设置上限
            backoff = std::min(backoff * 2, 1024);
        }
    }

    void unlock() {
        flag.store(false, std::memory_order_release);
    }

    bool try_lock() {
        return !flag.exchange(true, std::memory_order_acquire);
    }
};
```

3. **有限自旋后转为互斥锁的混合实现**：

```cpp
class HybridLock {
private:
    std::atomic<bool> flag{false};
    std::mutex mutex; // 作为回退策略的互斥锁

public:
    void lock() {
        // 先尝试快速路径 - 自旋几次
        for (int i = 0; i < 30; ++i) {
            if (!flag.exchange(true, std::memory_order_acquire)) {
                return;
            }

            // 添加CPU提示
            #if defined(__x86_64__) || defined(__i386__)
                __builtin_ia32_pause();
            #else
                std::this_thread::yield();
            #endif
        }

        // 自旋失败后，使用互斥锁(慢路径)
        mutex.lock();
        while (flag.exchange(true, std::memory_order_acquire)) {
            mutex.unlock();
            std::this_thread::yield();
            mutex.lock();
        }
    }

    void unlock() {
        flag.store(false, std::memory_order_release);
        // 不需要解锁mutex，因为在成功获取自旋锁后已经解锁了
    }
};
```

### 5.4 详细示例

以下是一个更完整的自旋锁使用示例，展示了在不同场景下的性能特点：

```cpp
#include <atomic>
#include <mutex>
#include <thread>
#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <numeric>

// 简单自旋锁实现
class SpinLock {
private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT;

public:
    void lock() {
        while (flag.test_and_set(std::memory_order_acquire));
    }

    void unlock() {
        flag.clear(std::memory_order_release);
    }
};

// 带退避的自旋锁实现
class BackoffSpinLock {
private:
    std::atomic<bool> flag{false};

public:
    void lock() {
        int backoff = 1;
        while (true) {
            if (!flag.exchange(true, std::memory_order_acquire)) {
                return;
            }

            // 指数退避
            for (int i = 0; i < backoff; ++i) {
                // 提示CPU我们在自旋
                std::this_thread::yield();
            }

            // 增加退避时间，但设置上限
            backoff = std::min(backoff * 2, 1024);
        }
    }

    void unlock() {
        flag.store(false, std::memory_order_release);
    }
};

// 用于测试的共享资源访问模式
enum class WorkMode {
    ShortCriticalSection,  // 短临界区
    MediumCriticalSection, // 中等临界区
    LongCriticalSection    // 长临界区
};

// 测试不同类型锁的性能
template<typename LockType>
double benchmark(int num_threads, WorkMode mode, int operations_per_thread) {
    LockType lock;
    int counter = 0;
    std::vector<std::thread> threads;
    std::atomic<bool> start{false};

    auto thread_func = [&](int id) {
        // 等待所有线程就绪
        while (!start.load());

        for (int i = 0; i < operations_per_thread; ++i) {
            lock.lock();

            // 执行临界区工作
            ++counter;

            // 模拟不同长度的临界区
            switch (mode) {
                case WorkMode::ShortCriticalSection:
                    // 非常短的临界区
                    break;

                case WorkMode::MediumCriticalSection:
                    // 中等长度的临界区 - 一些CPU指令
                    for (volatile int j = 0; j < 100; ++j);
                    break;

                case WorkMode::LongCriticalSection:
                    // 长临界区 - 包括一些计算
                    for (volatile int j = 0; j < 1000; ++j);
                    break;
            }

            lock.unlock();

            // 临界区外的工作
            for (volatile int j = 0; j < 10; ++j);
        }
    };

    // 创建并启动线程
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(thread_func, i);
    }

    // 开始计时
    auto start_time = std::chrono::high_resolution_clock::now();
    start.store(true);

    // 等待所有线程完成
    for (auto& t : threads) {
        t.join();
    }

    // 计算耗时
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end_time - start_time;

    std::cout << "计数器最终值: " << counter << " (期望: "
              << num_threads * operations_per_thread << ")" << std::endl;

    return elapsed.count();
}

int main() {
    const int OPERATIONS = 100000;
    const int REPEAT = 3;

    std::cout << "测试不同类型锁在各种工作负载下的性能\n";
    std::cout << "=======================================\n";

    // 测试不同数量的线程
    for (int num_threads : {1, 2, 4, 8, 16}) {
        std::cout << "\n线程数: " << num_threads << std::endl;
        std::cout << "---------------------------------------\n";

        // 测试不同模式
        for (auto mode : {WorkMode::ShortCriticalSection,
                           WorkMode::MediumCriticalSection,
                           WorkMode::LongCriticalSection}) {
            std::string mode_name;
            switch (mode) {
                case WorkMode::ShortCriticalSection:
                    mode_name = "短临界区";
                    break;
                case WorkMode::MediumCriticalSection:
                    mode_name = "中等临界区";
                    break;
                case WorkMode::LongCriticalSection:
                    mode_name = "长临界区";
                    break;
            }

            std::cout << "模式: " << mode_name << std::endl;

            // 重复多次取平均值
            std::vector<double> spin_times;
            std::vector<double> backoff_times;
            std::vector<double> mutex_times;

            for (int i = 0; i < REPEAT; ++i) {
                spin_times.push_back(benchmark<SpinLock>(
                    num_threads, mode, OPERATIONS / num_threads));

                backoff_times.push_back(benchmark<BackoffSpinLock>(
                    num_threads, mode, OPERATIONS / num_threads));

                mutex_times.push_back(benchmark<std::mutex>(
                    num_threads, mode, OPERATIONS / num_threads));
            }

            // 计算平均耗时
            double spin_avg = std::accumulate(
                spin_times.begin(), spin_times.end(), 0.0) / REPEAT;

            double backoff_avg = std::accumulate(
                backoff_times.begin(), backoff_times.end(), 0.0) / REPEAT;

            double mutex_avg = std::accumulate(
                mutex_times.begin(), mutex_times.end(), 0.0) / REPEAT;

            std::cout << "简单自旋锁: " << spin_avg << " ms\n";
            std::cout << "退避自旋锁: " << backoff_avg << " ms\n";
            std::cout << "互斥锁: " << mutex_avg << " ms\n";
            std::cout << std::endl;
        }
    }

    return 0;
}
```

### 5.5 性能考虑

自旋锁的性能特点很显著：

1. **临界区持续时间**：

   - 短临界区：自旋锁通常优于互斥锁
   - 长临界区：互斥锁通常优于自旋锁
2. **上下文切换成本**：

   - 系统负载轻(CPU空闲)：上下文切换开销较小，互斥锁可能更好
   - 系统负载重(CPU繁忙)：上下文切换开销大，自旋锁可能更好
3. **竞争程度**：

   - 低竞争：自旋和互斥性能接近
   - 高竞争：性能差异显著，取决于临界区长度
4. **硬件因素**：

   - CPU核心数：多核系统更适合自旋锁
   - CPU缓存一致性：跨处理器自旋可能受到缓存一致性协议的影响
5. **能源效率**：

   - 自旋锁会持续消耗CPU资源
   - 在电池供电设备上，互斥锁可能更节能

### 5.6 退避策略

为避免自旋锁的性能问题，可以使用多种退避策略：

1. **固定退避**：在每次尝试获取锁之间等待固定时间。
2. **指数退避**：等待时间随着失败尝试次数成指数增长，最多不超过上限。
3. **随机退避**：等待时间包含随机组件，类似以太网的CSMA/CD冲突解决方案。
4. **自适应退避**：根据先前的测量动态调整等待时间。
5. **CPU特定指令**：如x86的 `PAUSE`指令，可以改善自旋循环行为：

   - 减少内存顺序冲突
   - 减少电力消耗
   - 更高效地使用执行资源

### 5.7 自旋锁的缺点

1. **CPU消耗**：自旋锁持续消耗CPU资源，即使没有做有用工作。
2. **优先级反转**：低优先级线程持有锁时，高优先级线程会一直自旋浪费CPU。
3. **可伸缩性**：在高度竞争的场景下，自旋锁性能可能极差。
4. **不适合单核系统**：在单核系统上，持有锁的线程不可能同时运行，自旋完全是浪费。
5. **难以调试**：自旋锁问题可能导致CPU使用率接近100%，但没有明显症状。

### 5.8 适用场景

自旋锁最适合以下场景：

1. **超短临界区**：锁持有时间极短(微秒级)的场景。
2. **实时系统**：不能容忍线程被挂起和不可预测调度延迟的场景。
3. **高性能计算**：系统中线程数不超过CPU核心数，且临界区短。
4. **底层系统组件**：如操作系统内核、内存分配器等不能使用阻塞操作的环境。
5. **中断处理程序**：不能被阻塞的中断上下文。

### 5.9 自旋锁变体

除了基本自旋锁，还有一些特殊变体：

1. **读写自旋锁**：类似读写锁，但基于自旋实现的版本。
2. **票据锁(Ticket Lock)**：确保先到先得的公平性，减轻锁竞争：

```cpp
class TicketLock {
private:
    std::atomic<uint32_t> next_ticket{0};
    std::atomic<uint32_t> now_serving{0};

public:
    void lock() {
        // 取票
        uint32_t my_ticket = next_ticket.fetch_add(1, std::memory_order_relaxed);

        // 等待轮到自己
        while (now_serving.load(std::memory_order_acquire) != my_ticket) {
            // 适当退避
            std::this_thread::yield();
        }
    }

    void unlock() {
        // 增加正在服务的票号
        now_serving.fetch_add(1, std::memory_order_release);
    }
};
```

3. **MCS锁**：一种基于队列的自旋锁，减少共享缓存行的争用：

```cpp
class MCSLock {
private:
    struct Node {
        Node* next{nullptr};
        bool locked{false};
    };

    std::atomic<Node*> tail{nullptr};

public:
    void lock(Node& my_node) {
        my_node.next = nullptr;
        my_node.locked = true;

        // 将自己加入队列尾部，并获取前一个节点
        Node* predecessor = tail.exchange(&my_node, std::memory_order_acq_rel);

        if (predecessor != nullptr) {
            // 队列不为空，需要等待
            predecessor->next = &my_node;

            // 自旋等待前一个节点通知
            while (my_node.locked) {
                std::this_thread::yield();
            }
        }
    }

    void unlock(Node& my_node) {
        if (my_node.next == nullptr) {
            // 尝试将尾部设置为空
            Node* expected = &my_node;
            if (tail.compare_exchange_strong(expected, nullptr, std::memory_order_release)) {
                // 成功删除，队列现在为空
                return;
            }

            // 有新节点加入，等待它完成链接
            while (my_node.next == nullptr) {
                std::this_thread::yield();
            }
        }

        // 通知下一个等待者
        my_node.next->locked = false;
    }
};
```

4. **CLH锁**：另一种队列自旋锁，优化了缓存一致性：

```cpp
class CLHLock {
private:
    struct Node {
        std::atomic<bool> locked{true};
    };

    std::atomic<Node*> tail;
    thread_local static Node* my_pred;
    thread_local static Node* my_node;

public:
    CLHLock() : tail(new Node{false}) {} // 初始节点

    void lock() {
        my_node = new Node{true};
        my_pred = tail.exchange(my_node, std::memory_order_acq_rel);

        // 等待前一个节点释放
        while (my_pred->locked.load(std::memory_order_acquire)) {
            std::this_thread::yield();
        }
    }

    void unlock() {
        my_node->locked.store(false, std::memory_order_release);

        // 使用前一个节点作为下次的新节点，回收当前节点
        Node* temp = my_node;
        my_node = my_pred;
        delete temp;
    }
};
```

### 5.10 最佳实践

使用自旋锁时的建议：

1. **确保临界区极短**：理想情况下，临界区应该只有几个汇编指令。
2. **避免在持有自旋锁时**：

   - 进行阻塞操作
   - 执行复杂计算
   - 调用未知函数
   - 获取其他锁
3. **使用混合策略**：在特定自旋次数后回退到互斥锁。
4. **测量再决定**：在实际环境中测试性能，再决定是否使用自旋锁。
5. **使用正确的内存顺序**：自旋锁实现中使用适当的内存栅栏和顺序约束。

`<a id="信号量"></a>`

## 6. 信号量(Semaphore)

### 6.1 基本概念

信号量是一种更为通用的同步原语，由荷兰计算机科学家Edsger Dijkstra于1965年发明。信号量维护一个内部计数器，表示可用资源的数量。它支持两种基本操作：

- **P操作(也称wait或acquire)**：尝试减少计数器的值，如果计数器为0，则阻塞
- **V操作(也称signal或release)**：增加计数器的值，可能唤醒等待的线程

信号量分为两种类型：

1. **二进制信号量**：计数器只有0和1两个值，功能类似互斥锁
2. **计数信号量**：计数器可以有任意非负值，用于控制对有限资源的访问

### 6.2 底层实现原理

信号量的典型实现包括：

1. **数据结构**：

   - 计数器：表示当前可用资源数
   - 等待队列：存储因资源不足而阻塞的线程
   - 互斥锁：保护内部状态的并发访问
2. **核心操作**：

   - **P操作(acquire)**：

     ```
     原子操作 {
       if (计数器 > 0) {
         计数器 -= 1;
         返回成功;
       } else {
         将当前线程加入等待队列;
         阻塞当前线程;
       }
     }
     ```
   - **V操作(release)**：

     ```
     原子操作 {
       计数器 += 1;
       if (等待队列非空) {
         从队列取出一个线程;
         唤醒该线程;
       }
     }
     ```
3. **POSIX实现**：

   - 基于 `sem_t`结构
   - 提供 `sem_wait`、`sem_post`和 `sem_init`等API
   - 内部使用原子操作和线程调度机制实现
4. **Windows实现**：

   - 使用 `CreateSemaphore`创建的内核对象
   - 提供 `WaitForSingleObject`和 `ReleaseSemaphore`等API
   - 由操作系统内核管理计数和等待队列

### 6.3 C++中的信号量

1. **C++20之前**：
   C++标准库在C++20之前不提供内置的信号量，需要使用系统API或自行实现：

   ```cpp
   // 使用互斥锁和条件变量实现的信号量
   class Semaphore {
   private:
       int count_;
       std::mutex mutex_;
       std::condition_variable cv_;

   public:
       explicit Semaphore(int count = 0) : count_(count) {}

       void acquire() {
           std::unique_lock<std::mutex> lock(mutex_);
           cv_.wait(lock, [this]{ return count_ > 0; });
           --count_;
       }

       bool try_acquire() {
           std::lock_guard<std::mutex> lock(mutex_);
           if (count_ > 0) {
               --count_;
               return true;
           }
           return false;
       }

       void release() {
           std::lock_guard<std::mutex> lock(mutex_);
           ++count_;
           cv_.notify_one();
       }
   };
   ```
2. **C++20标准**：
   C++20引入了标准的信号量实现，位于 `<semaphore>`头文件中：

   - **`std::counting_semaphore<>`**：通用的计数信号量
   - **`std::binary_semaphore`**：二进制信号量，是 `counting_semaphore<1>`的别名

   标准信号量提供以下操作：

   - `acquire()`：尝试获取一个资源，必要时阻塞
   - `try_acquire()`：尝试非阻塞获取资源
   - `try_acquire_for()`：带超时的获取
   - `try_acquire_until()`：获取直到某个时间点
   - `release(n = 1)`：释放n个资源

   ```cpp
   #include <semaphore>

   // 创建初始值为5的计数信号量
   std::counting_semaphore<5> sem(5);

   // 创建二进制信号量(初始值为1)
   std::binary_semaphore bin_sem(1);
   ```

### 6.4 典型用例

信号量有多种经典应用场景：

1. **资源计数与限制**：

```cpp
// 限制最多有3个线程同时访问资源池
std::counting_semaphore<3> pool_sem(3);

void worker() {
    pool_sem.acquire();  // 获取资源

    // 使用受限资源...

    pool_sem.release();  // 释放资源
}
```

2. **线程协调与同步**：

```cpp
// 使用二进制信号量实现任务完成信号
std::binary_semaphore task_done(0);  // 初始值为0表示未完成

void worker() {
    // 执行任务...

    // 通知任务完成
    task_done.release();
}

void supervisor() {
    // 等待任务完成
    task_done.acquire();

    // 处理任务结果...
}
```

3. **生产者-消费者模式**：

```cpp
std::counting_semaphore<BUFFER_SIZE> empty(BUFFER_SIZE); // 空槽位数
std::counting_semaphore<BUFFER_SIZE> filled(0);          // 满槽位数
std::mutex buffer_mutex;                                 // 保护缓冲区访问

void producer() {
    while (true) {
        // 生产项目...

        empty.acquire();  // 等待空槽位

        {
            std::lock_guard<std::mutex> lock(buffer_mutex);
            // 将项目添加到缓冲区...
        }

        filled.release(); // 增加满槽位计数
    }
}

void consumer() {
    while (true) {
        filled.acquire(); // 等待满槽位

        {
            std::lock_guard<std::mutex> lock(buffer_mutex);
            // 从缓冲区取出项目...
        }

        empty.release();  // 增加空槽位计数

        // 处理项目...
    }
}
```

### 6.5 详细示例

以下是一个使用信号量实现的有限连接池示例，适用于C++20：

```cpp
#include <semaphore>
#include <mutex>
#include <thread>
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <functional>
#include <string>

// 模拟数据库连接类
class DBConnection {
public:
    explicit DBConnection(int id) : id_(id) {
        std::cout << "创建数据库连接 #" << id_ << std::endl;
    }

    ~DBConnection() {
        std::cout << "关闭数据库连接 #" << id_ << std::endl;
    }

    void query(const std::string& sql) {
        std::cout << "连接 #" << id_ << " 执行查询: " << sql << std::endl;
        // 模拟查询耗时
        std::this_thread::sleep_for(std::chrono::milliseconds(50 + id_ * 10));
    }

private:
    int id_;
};

// 数据库连接池
class DBConnectionPool {
public:
    explicit DBConnectionPool(int pool_size)
        : pool_size_(pool_size), available_connections_(pool_size) {

        // 预创建所有连接
        for (int i = 0; i < pool_size; ++i) {
            connections_.push_back(std::make_unique<DBConnection>(i + 1));
        }

        // 所有连接初始为可用
        for (int i = 0; i < pool_size; ++i) {
            available_indices_.push_back(i);
        }

        std::cout << "连接池初始化完成，大小: " << pool_size_ << std::endl;
    }

    // 借用一个连接(可能阻塞)
    DBConnection* borrow() {
        // 等待有可用连接
        available_connections_.acquire();

        // 获取一个可用连接的索引
        std::lock_guard<std::mutex> lock(mutex_);
        int idx = available_indices_.back();
        available_indices_.pop_back();

        std::cout << "借出连接 #" << (idx + 1) << ", 剩余可用: "
                  << available_indices_.size() << std::endl;

        return connections_[idx].get();
    }

    // 尝试借用一个连接(非阻塞)
    DBConnection* try_borrow() {
        if (!available_connections_.try_acquire()) {
            return nullptr;  // 没有可用连接
        }

        // 获取一个可用连接
        std::lock_guard<std::mutex> lock(mutex_);
        int idx = available_indices_.back();
        available_indices_.pop_back();

        std::cout << "非阻塞借出连接 #" << (idx + 1) << ", 剩余可用: "
                  << available_indices_.size() << std::endl;

        return connections_[idx].get();
    }

    // 带超时的借用
    DBConnection* try_borrow_for(std::chrono::milliseconds timeout) {
        if (!available_connections_.try_acquire_for(timeout)) {
            return nullptr;  // 超时
        }

        // 获取一个可用连接
        std::lock_guard<std::mutex> lock(mutex_);
        int idx = available_indices_.back();
        available_indices_.pop_back();

        std::cout << "超时借出连接 #" << (idx + 1) << ", 剩余可用: "
                  << available_indices_.size() << std::endl;

        return connections_[idx].get();
    }

    // 归还连接
    void return_connection(DBConnection* connection) {
        std::lock_guard<std::mutex> lock(mutex_);

        // 查找连接在池中的索引
        for (int i = 0; i < pool_size_; ++i) {
            if (connections_[i].get() == connection) {
                available_indices_.push_back(i);
                std::cout << "归还连接 #" << (i + 1) << ", 现在可用: "
                          << available_indices_.size() << std::endl;
                break;
            }
        }

        // 增加可用连接计数
        available_connections_.release();
    }

    // 当前可用连接数
    int available() const {
        // 注意：这是不准确的，因为非原子操作
        return available_indices_.size();
    }

private:
    int pool_size_;
    std::vector<std::unique_ptr<DBConnection>> connections_;
    std::vector<int> available_indices_;
    std::mutex mutex_;
    std::counting_semaphore<> available_connections_;
};

// 使用连接池的示例
int main() {
    // 创建一个有5个连接的池
    DBConnectionPool pool(5);

    // 创建10个工作线程
    std::vector<std::thread> workers;
    for (int i = 0; i < 10; ++i) {
        workers.push_back(std::thread([&pool, i]() {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> delay_dist(100, 500);

            // 每个线程尝试执行3次查询
            for (int j = 0; j < 3; ++j) {
                std::cout << "线程 " << i << " 尝试获取连接..." << std::endl;

                // 不同线程使用不同的连接获取策略
                DBConnection* conn = nullptr;
                if (i % 3 == 0) {
                    // 阻塞模式
                    conn = pool.borrow();
                    std::cout << "线程 " << i << " 阻塞模式获取连接成功" << std::endl;
                } else if (i % 3 == 1) {
                    // 非阻塞模式
                    while (!(conn = pool.try_borrow())) {
                        std::cout << "线程 " << i << " 无可用连接，稍后重试..." << std::endl;
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    }
                    std::cout << "线程 " << i << " 非阻塞模式获取连接成功" << std::endl;
                } else {
                    // 超时模式
                    while (!(conn = pool.try_borrow_for(std::chrono::milliseconds(200)))) {
                        std::cout << "线程 " << i << " 获取连接超时，重试..." << std::endl;
                    }
                    std::cout << "线程 " << i << " 超时模式获取连接成功" << std::endl;
                }

                // 使用连接执行查询
                std::string query = "SELECT * FROM table_" + std::to_string(i) +
                                    " WHERE id = " + std::to_string(j);
                conn->query(query);

                // 随机延迟一段时间后归还连接
                std::this_thread::sleep_for(std::chrono::milliseconds(delay_dist(gen)));
                pool.return_connection(conn);

                // 线程工作间隔
                std::this_thread::sleep_for(std::chrono::milliseconds(delay_dist(gen)));
            }
        }));
    }

    // 等待所有工作线程完成
    for (auto& worker : workers) {
        worker.join();
    }

    std::cout << "所有线程完成，程序退出" << std::endl;

    return 0;
}
```

对于C++20之前的环境，可以使用自定义实现：

```cpp
// 用于C++17环境的等效实现
class Semaphore {
private:
    int count_;
    std::mutex mutex_;
    std::condition_variable cv_;

public:
    explicit Semaphore(int count = 0) : count_(count) {}

    void acquire() {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this]{ return count_ > 0; });
        --count_;
    }

    bool try_acquire() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (count_ > 0) {
            --count_;
            return true;
        }
        return false;
    }

    template<class Rep, class Period>
    bool try_acquire_for(const std::chrono::duration<Rep, Period>& timeout) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (!cv_.wait_for(lock, timeout, [this]{ return count_ > 0; })) {
            return false;
        }
        --count_;
        return true;
    }

    void release() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            ++count_;
        }
        cv_.notify_one();
    }
};

// 然后在DBConnectionPool中使用这个Semaphore类
```

### 6.6 性能考虑

信号量操作的性能特点：

1. **P操作(acquire)**：

   - 当资源可用时：只涉及原子计数器减少，性能接近互斥锁获取
   - 当需要阻塞时：涉及线程挂起操作，开销与互斥锁阻塞类似
2. **V操作(release)**：

   - 当没有等待线程时：只涉及原子计数器增加，开销很小
   - 当需要唤醒线程时：涉及线程唤醒操作，开销较大
3. **二进制信号量 vs 互斥锁**：

   - 二进制信号量通常比互斥锁开销略大
   - 互斥锁通常针对常见的锁定模式有更多优化
4. **计数信号量**：

   - 计数器操作通常是原子的，因此性能良好
   - 大规模并发更新可能导致缓存一致性开销
5. **系统调用开销**：

   - 在某些系统上，信号量操作可能涉及系统调用
   - 与用户空间实现相比，可能有额外开销

### 6.7 信号量的缺点

信号量虽然功能强大，但也有一些明显的缺点：

1. **使用复杂性**：信号量语义不如互斥锁直观，使用错误可能导致难以发现的bug。
2. **不遵循RAII**：原生信号量API不支持C++的RAII模式，需要额外封装。
3. **死锁风险**：不正确使用信号量可能导致死锁，特别是在使用多个信号量时。
4. **欠缺所有权概念**：信号量没有线程所有权概念，任何线程都可以释放资源，可能导致混乱。
5. **调试困难**：信号量相关的并发问题通常难以调试。
6. **性能开销**：在简单场景下，信号量可能比更专用的同步原语(如互斥锁)有更多开销。

### 6.8 适用场景

信号量最适合以下场景：

1. **资源池管理**：限制对有限数量资源的并发访问，如数据库连接池、线程池。
2. **生产者-消费者问题**：协调生产者和消费者之间的关系，保证缓冲区正确访问。
3. **多任务协调**：需要精确控制多个任务执行顺序的场景。
4. **有限并发控制**：限制特定操作的并发程度，如网络请求数量限制。
5. **读者-写者问题的一种解决方案**：使用多个信号量协调读写访问。

### 6.9 最佳实践

使用信号量时的最佳实践：

1. **优先使用标准库**：C++20环境下优先使用 `std::counting_semaphore`。
2. **创建RAII包装器**：为获取/释放操作创建RAII包装，确保异常安全：

```cpp
template<typename Semaphore>
class SemaphoreGuard {
private:
    Semaphore& sem_;
    bool active_ = true;

public:
    explicit SemaphoreGuard(Semaphore& sem) : sem_(sem) {
        sem_.acquire();
    }

    ~SemaphoreGuard() {
        if (active_) {
            sem_.release();
        }
    }

    void release() {
        if (active_) {
            sem_.release();
            active_ = false;
        }
    }

    // 禁止复制
    SemaphoreGuard(const SemaphoreGuard&) = delete;
    SemaphoreGuard& operator=(const SemaphoreGuard&) = delete;
};
```

3. **避免复杂的信号量交互**：多个信号量相互依赖的设计容易导致死锁。
4. **初始值设置正确**：根据资源的实际数量设置正确的初始值。
5. **评估替代方案**：考虑是否有更专用的同步原语(如互斥锁、条件变量)更适合问题。
6. **明确计数语义**：确保对信号量计数如何映射到实际资源有清晰的理解。
7. **文档化信号量用途**：清晰记录每个信号量的目的、初始值含义和使用模式。

`<a id="原子操作"></a>`

## 7. 原子操作(Atomic Operations)

### 7.1 基本概念

原子操作是不可分割的操作单元，在执行过程中不会被线程调度机制中断，也不会被其他线程观察到部分完成的状态。原子操作是构建无锁(lock-free)数据结构和算法的基础，也是实现其他同步原语的底层机制。

与传统锁相比，原子操作具有以下特点：

- 不需要获取/释放锁的开销
- 避免了线程调度和上下文切换
- 通常性能更高，特别是在低竞争场景下
- 可以避免死锁、优先级反转等锁相关问题

### 7.2 底层实现原理

原子操作的实现依赖于硬件和编译器的支持：

1. **硬件支持**：

   - **原子指令**：现代CPU提供特殊的原子指令，如：

     - x86: LOCK前缀、XCHG、CMPXCHG(比较并交换)
     - ARM: LDREX/STREX(加载/存储独占)指令对
     - PowerPC: Load-and-Reserve/Store-Conditional指令对
   - **内存屏障(Memory Barriers)**：确保内存访问按预期顺序完成的特殊指令
   - **缓存一致性协议**：如MESI，确保多核系统中所有核心看到一致的内存状态
2. **软件实现**：

   - **编译器内建函数**：现代编译器提供原子操作内建函数
   - **指令重排限制**：防止编译器和CPU重排序影响原子性
   - **内存模型支持**：定义多线程程序中内存访问的规则
3. **不同类型操作的实现**：

   - **简单加载/存储**：对于自然对齐的字长内的数据，可能直接映射到单个CPU指令
   - **比较并交换(CAS)**：实现原子更新的基础操作
   - **读-修改-写(RMW)**：如原子递增，通常使用专用硬件指令或CAS循环实现

### 7.3 C++中的原子操作

C++11引入了原子操作库，位于 `<atomic>`头文件中：

1. **主要类型**：

   - **`std::atomic<T>`**：通用原子类型模板
   - **特化类型**：如 `std::atomic_bool`、`std::atomic_int`等
   - **`std::atomic_flag`**：最简单的原子布尔类型，保证无锁实现
2. **主要操作**：

   - **加载(load)**：原子读取值
   - **存储(store)**：原子写入值
   - **交换(exchange)**：原子替换值并返回旧值
   - **比较交换(compare_exchange)**：条件更新
   - **算术操作**：如 `fetch_add`、`fetch_sub`等
   - **位操作**：如 `fetch_and`、`fetch_or`、`fetch_xor`
   - **类条件变量** 如 `wait`、`notify_one`、`notify_all`
3. **内存序(Memory Ordering)**：
   C++原子库允许指定操作的内存顺序：

   - **`memory_order_relaxed`**：仅保证操作的原子性，无额外同步
   - **`memory_order_consume`**：已被废弃，通常使用 `acquire`代替
   - **`memory_order_acquire`**：读取操作，建立后续读取的屏障
   - **`memory_order_release`**：写入操作，建立之前写入的屏障
   - **`memory_order_acq_rel`**：组合 `acquire`和 `release`语义
   - **`memory_order_seq_cst`**：最严格的顺序，保证所有线程看到一致的操作顺序

### 7.4 原子操作的基本用例

1. **简单计数器**：

```cpp
std::atomic<int> counter{0};

void increment() {
    ++counter;  // 原子递增
}

void reset() {
    counter.store(0);  // 原子存储
}

int get() {
    return counter.load();  // 原子加载
}
```

2. **线程安全的单次初始化**：

```cpp
std::atomic<bool> initialized{false};
std::string data;  // 需要初始化的数据

void init() {
    if (!initialized.load(std::memory_order_acquire)) {
        // 双重检查锁定模式
        static std::mutex mutex;
        std::lock_guard<std::mutex> lock(mutex);

        if (!initialized.load(std::memory_order_relaxed)) {
            // 进行复杂初始化
            data = "大量数据...";

            // 确保初始化对其他线程可见
            initialized.store(true, std::memory_order_release);
        }
    }
}

void use_data() {
    init();  // 确保数据已初始化
    // 使用data...
}
```

3. **无锁数据交换**：

```cpp
std::atomic<int*> ptr{nullptr};
int data[2] = {1, 2};

void producer() {
    // 准备新数据
    int* new_data = new int[2]{3, 4};

    // 原子交换指针，获取旧指针
    int* old_data = ptr.exchange(new_data);

    // 清理旧数据(如果存在)
    delete[] old_data;
}

void consumer() {
    // 原子读取当前指针
    int* current = ptr.load();
    if (current) {
        // 使用数据
        std::cout << "值: " << current[0] << ", " << current[1] << std::endl;
    }
}
```

### 7.5 详细示例：无锁队列

以下是使用原子操作实现的简单单生产者-单消费者无锁队列：

```cpp
#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

template<typename T>
class LockFreeQueue {
private:
    struct Node {
        T data;
        Node* next;

        Node(const T& val) : data(val), next(nullptr) {}
    };

    std::atomic<Node*> head_;
    std::atomic<Node*> tail_;

public:
    LockFreeQueue() {
        // 创建哨兵节点
        Node* dummy = new Node(T{});
        head_.store(dummy);
        tail_.store(dummy);
    }

    ~LockFreeQueue() {
        // 清理所有节点
        Node* current = head_.load();
        while (current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

    // 入队操作(单生产者假设)
    void enqueue(const T& value) {
        // 创建新节点
        Node* new_node = new Node(value);

        // 获取当前尾节点
        Node* current_tail = tail_.load();

        // 将新节点链接到尾部
        current_tail->next = new_node;

        // 更新尾指针
        tail_.store(new_node);
    }

    // 出队操作(单消费者假设)
    bool dequeue(T& result) {
        // 获取头节点(哨兵)和下一个节点
        Node* current_head = head_.load();
        Node* next_node = current_head->next;

        // 队列为空
        if (!next_node) {
            return false;
        }

        // 获取数据
        result = next_node->data;

        // 更新头指针
        head_.store(next_node);

        // 删除旧的哨兵节点
        delete current_head;

        return true;
    }

    // 检查队列是否为空
    bool empty() const {
        return head_.load() == tail_.load();
    }
};

// 使用示例
int main() {
    // 创建队列
    LockFreeQueue<int> queue;

    // 生产者线程
    std::thread producer([&]() {
        for (int i = 1; i <= 1000000; ++i) {
            queue.enqueue(i);
        }
    });

    // 消费者线程
    std::thread consumer([&]() {
        int sum = 0;
        int count = 0;
        int value;

        // 持续尝试出队
        while (count < 1000000) {
            if (queue.dequeue(value)) {
                sum += value;
                ++count;
            } else {
                // 避免忙等待
                std::this_thread::yield();
            }
        }

        std::cout << "总和: " << sum << std::endl;
    });

    // 等待线程完成
    producer.join();
    consumer.join();

    return 0;
}
```

这个简化的无锁队列只适用于单生产者-单消费者场景。多生产者或多消费者情况需要更复杂的实现，通常使用CAS操作确保线程安全。

### 7.6 更复杂的示例：ABA问题与解决方案

原子操作中的一个经典问题是"ABA问题"，当使用比较并交换(CAS)操作时，如果一个值从A变为B再变回A，CAS操作可能无法检测到这种变化：

```cpp
#include <atomic>
#include <thread>
#include <iostream>
#include <vector>

// 演示ABA问题的示例
void demonstrate_aba_problem() {
    std::atomic<int*> ptr(new int(1));
    int* old_value = ptr.load();

    // 线程1读取ptr，但暂时不进行CAS
    std::thread t1([&]() {
        // 模拟一些延迟
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        int* expected = old_value;
        int* new_value = new int(3);

        // CAS操作 - 如果ptr仍然是old_value，则更新为new_value
        bool success = ptr.compare_exchange_strong(expected, new_value);

        std::cout << "线程1 CAS " << (success ? "成功" : "失败") << std::endl;
        if (!success) {
            delete new_value;  // 清理资源
            std::cout << "线程1 预期:" << *expected << " 实际:" << *(ptr.load()) << std::endl;
        }
    });

    // 线程2在线程1执行CAS前改变ptr
    std::thread t2([&]() {
        // 将ptr从A变为B
        int* temp = new int(2);
        int* old = ptr.exchange(temp);
        std::cout << "线程2 将值从 " << *old << " 改为 " << *temp << std::endl;
        delete old;

        // 短暂延迟
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        // 再将ptr从B变回A
        int* new_ptr = new int(1);  // 与原始值相同的新对象
        old = ptr.exchange(new_ptr);
        std::cout << "线程2 将值从 " << *old << " 改回 " << *new_ptr << std::endl;
        delete old;
    });

    t1.join();
    t2.join();

    // 清理
    delete ptr.load();
}

// 使用带标记的指针解决ABA问题
struct TaggedPointer {
    void* ptr;
    unsigned long tag;
};

class AtomicTaggedPointer {
private:
    std::atomic<TaggedPointer*> atomic_ptr_;

public:
    AtomicTaggedPointer(void* initial_ptr = nullptr) {
        TaggedPointer* p = new TaggedPointer{initial_ptr, 0};
        atomic_ptr_.store(p);
    }

    ~AtomicTaggedPointer() {
        delete atomic_ptr_.load();
    }

    bool compare_exchange_strong(void* expected_ptr, void* new_ptr) {
        TaggedPointer* expected = atomic_ptr_.load();

        if (expected->ptr != expected_ptr) {
            return false;
        }

        TaggedPointer* new_tagged = new TaggedPointer{
            new_ptr,
            expected->tag + 1  // 增加标记计数
        };

        bool success = atomic_ptr_.compare_exchange_strong(expected, new_tagged);

        if (success) {
            delete expected;
        } else {
            delete new_tagged;
        }

        return success;
    }

    void* load() {
        return atomic_ptr_.load()->ptr;
    }

    unsigned long get_tag() {
        return atomic_ptr_.load()->tag;
    }

    void* exchange(void* new_ptr) {
        TaggedPointer* old = atomic_ptr_.load();
        TaggedPointer* new_tagged = new TaggedPointer{
            new_ptr,
            old->tag + 1  // 增加标记计数
        };

        old = atomic_ptr_.exchange(new_tagged);
        void* result = old->ptr;
        delete old;

        return result;
    }
};

// 使用标记指针解决ABA问题的示例
void solve_aba_problem() {
    AtomicTaggedPointer ptr(new int(1));
    void* old_value = ptr.load();

    std::thread t1([&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        int* new_value = new int(3);
        bool success = ptr.compare_exchange_strong(old_value, new_value);

        std::cout << "带标记 - 线程1 CAS " << (success ? "成功" : "失败") << std::endl;
        if (!success) {
            delete new_value;
        }
    });

    std::thread t2([&]() {
        int* temp = new int(2);
        int* old = static_cast<int*>(ptr.exchange(temp));
        std::cout << "带标记 - 线程2 将值从 " << *old
                  << " 改为 " << *temp
                  << " (标记:" << ptr.get_tag() << ")" << std::endl;
        delete old;

        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        int* new_ptr = new int(1);
        old = static_cast<int*>(ptr.exchange(new_ptr));
        std::cout << "带标记 - 线程2 将值从 " << *old
                  << " 改回 " << *new_ptr
                  << " (标记:" << ptr.get_tag() << ")" << std::endl;
        delete old;
    });

    t1.join();
    t2.join();

    // 清理
    delete static_cast<int*>(ptr.load());
}

int main() {
    std::cout << "演示ABA问题:\n";
    demonstrate_aba_problem();

    std::cout << "\n使用标记指针解决ABA问题:\n";
    solve_aba_problem();

    return 0;
}
```

### 7.7 性能考虑

原子操作的性能特点：

1. **轻量级操作**：

   - 简单的原子加载/存储通常比互斥锁获取/释放快数倍
   - 无线程调度或上下文切换开销
2. **硬件依赖**：

   - 性能严重依赖底层硬件实现
   - 在支持原子指令的现代CPU上性能最佳
   - 对于不支持的操作，可能回退到使用全局锁
3. **缓存一致性影响**：

   - 原子操作通常导致CPU缓存一致性协议活动
   - 多CPU/核心间频繁的原子操作可能引起缓存颠簸
4. **内存序影响**：

   - `memory_order_relaxed`通常最快
   - `memory_order_seq_cst`通常最慢
   - 但具体性能差异依赖于硬件架构
5. **CAS循环开销**：

   - 在高竞争环境下，CAS循环可能多次失败，导致过多CPU消耗
   - 竞争严重时可能比互斥锁慢
6. **数据大小影响**：

   - 对于较大的数据结构，原子操作可能不如互斥锁效率高
   - 超过硬件原子支持大小的数据通常需要间接实现（如原子指针）

### 7.8 原子操作的局限性

虽然强大，但原子操作也有明显的局限性：

1. **复杂性**：

   - 正确使用原子操作需要深入理解内存模型
   - 无锁算法设计和验证难度高
2. **有限的组合性**：

   - 原子操作难以组合成更复杂的原子操作
   - 多步骤原子操作通常需要额外机制（如锁或事务内存）
3. **ABA问题**：

   - 比较并交换容易受到ABA问题影响
   - 需要额外机制（如标记指针）解决
4. **可扩展性挑战**：

   - 在高竞争下，原子操作性能可能迅速下降
   - 需要仔细设计避免热点竞争
5. **难以调试**：

   - 原子操作相关的并发问题难以重现和调试
   - 错误通常是微妙的时序问题
6. **硬件局限**：

   - 某些平台可能对特定类型没有真正的原子支持
   - 大型原子类型可能通过锁模拟，性能降低

### 7.9 适用场景

原子操作最适合以下场景：

1. **简单共享计数器**：如引用计数、统计计数器等。
2. **标志和状态变量**：如初始化标志、停止标志等。
3. **无锁数据结构**：如无锁队列、栈等，特别是在生产者-消费者模式中。
4. **自旋锁实现**：作为构建自旋锁等同步原语的基础。
5. **高性能并发系统**：在性能关键路径中替代互斥锁。
6. **实时系统**：需要避免不确定延迟的场景。
7. **底层同步原语**：实现其他同步机制的基础组件。

### 7.10 内存模型与内存序

C++内存模型是理解原子操作行为的关键：

1. **基本概念**：

   - **内存位置(Memory Location)**：可访问的存储单元
   - **数据竞争(Data Race)**：对同一内存位置的非同步并发访问
   - **可观察行为(Observable Behavior)**：程序执行的外部可见结果
   - **先行(Happens-before)关系**：操作之间的因果序列
2. **内存序选项的含义**：

   - **`memory_order_relaxed`**：

     - 仅保证原子性，不建立同步关系
     - 允许最大程度的重排序
     - 适用于：计数器、无需同步的状态累积
   - **`memory_order_acquire`**：

     - 在此加载之前的所有写入对获取加载的线程可见
     - 禁止读取操作向上重排
     - 适用于：获取锁、读取共享数据前
   - **`memory_order_release`**：

     - 在此存储之前的所有写入对执行获取操作的线程可见
     - 禁止写入操作向下重排
     - 适用于：释放锁、共享数据写入后
   - **`memory_order_acq_rel`**：

     - 组合获取和释放语义
     - 适用于：读-修改-写操作，如fetch_add
   - **`memory_order_seq_cst`**：

     - 全局一致的总顺序
     - 所有线程看到的操作顺序相同
     - 适用于：需要直观行为且性能不是关键因素时
3. **典型使用模式**：

```cpp
// 释放-获取同步模式
std::atomic<bool> flag{false};
int data = 0;

// 线程A
void producer() {
    data = 42;  // 准备数据
    // 释放操作确保上面的写入对执行获取操作的线程可见
    flag.store(true, std::memory_order_release);
}

// 线程B
void consumer() {
    // 获取操作同步与上面的释放操作
    while (!flag.load(std::memory_order_acquire)) {
        // 等待数据准备好
        std::this_thread::yield();
    }

    // 由于释放-获取同步，这里可以安全访问data
    assert(data == 42);
}
```

### 7.11 最佳实践

使用原子操作的一些最佳实践：

1. **从简单开始**：

   - 先使用互斥锁实现，再考虑原子操作优化
   - 避免一开始就设计复杂的无锁算法
2. **测量性能**：

   - 不要假设原子操作总是更快
   - 在实际条件下测量性能差异
3. **合理选择内存序**：

   - 默认使用顺序一致(`seq_cst`)，确保正确性
   - 性能关键路径上考虑使用更宽松的内存序
   - 记录并解释每个非默认内存序的选择
4. **关注数据设计**：

   - 避免伪共享(false sharing)
   - 使用缓存行填充隔离频繁修改的原子变量
   - 注意数据布局的缓存友好性
5. **处理ABA问题**：

   - 使用版本计数或标记指针
   - 考虑内存管理策略（如风险指针、屏障等）
6. **避免忙等待**：

   - 在长时间自旋中加入退避策略
   - 考虑与阻塞同步机制结合使用
7. **全面测试**：

   - 使用专门的并发测试工具
   - 考虑在多种硬件架构上测试
   - 压力测试在高负载下的行为

`<a id="超时锁"></a>`

## 8. 超时锁(Timed Mutex)

### 8.1 基本概念

超时锁是对基本互斥锁的扩展，它增加了尝试在指定时间内获取锁的能力。如果在指定时间内无法获取锁，则返回失败而不是无限期阻塞。这种机制有助于避免死锁，并使程序能够从长时间的锁等待中恢复。

C++标准库提供了两种基本的超时互斥锁类型：

- **`std::timed_mutex`**：带超时功能的基本互斥锁
- **`std::recursive_timed_mutex`**：带超时功能的递归互斥锁

### 8.2 底层实现原理

超时锁的实现通常基于以下机制：

1. **操作系统支持**：

   - **POSIX**：使用 `pthread_mutex_timedlock`函数
   - **Windows**：使用 `WaitForSingleObject`函数并指定超时值
2. **超时检测**：

   - 计算截止时间
   - 定期检查当前时间与截止时间的关系
   - 发现超时时终止等待并返回错误
3. **典型实现策略**：

   - **忙等待与休眠结合**：短时间内忙等待，长时间则使线程休眠
   - **条件变量超时**：使用条件变量的超时等待机制
   - **系统调用超时**：直接使用操作系统提供的超时锁定API

### 8.3 C++中的超时锁操作

C++超时锁提供以下独特的操作：

1. **`try_lock_for`**：

   - 尝试在指定的时间段(duration)内获取锁
   - 语法：`bool try_lock_for(const std::chrono::duration& rel_time)`
   - 成功获取锁返回true，超时返回false
2. **`try_lock_until`**：

   - 尝试在指定的时间点(time point)之前获取锁
   - 语法：`bool try_lock_until(const std::chrono::time_point& abs_time)`
   - 成功获取锁返回true，到达时间点仍未获取锁则返回false
3. **`std::unique_lock`支持的定时锁定**：

   - `std::unique_lock`提供对应的 `try_lock_for`和 `try_lock_until`方法
   - 支持RAII风格的超时锁定

### 8.4 详细示例

以下是超时锁的实际应用示例：

```cpp
#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>
#include <vector>
#include <string>
#include <atomic>

class ResourceManager {
private:
    std::timed_mutex resource_mutex_;
    std::atomic<int> access_count_{0};
    std::atomic<int> timeout_count_{0};

public:
    // 使用超时锁访问资源
    bool useResource(int thread_id, const std::string& operation,
                     std::chrono::milliseconds timeout) {

        // 尝试在指定时间内获取锁
        if (resource_mutex_.try_lock_for(timeout)) {
            // 获取锁成功
            ++access_count_;

            std::cout << "线程 " << thread_id << " 成功获取资源锁，执行: "
                      << operation << std::endl;

            // 模拟工作负载 - 根据线程ID变化工作时间
            std::this_thread::sleep_for(
                std::chrono::milliseconds(50 + thread_id * 20));

            std::cout << "线程 " << thread_id << " 完成操作: "
                      << operation << std::endl;

            // 释放锁
            resource_mutex_.unlock();
            return true;
        } else {
            // 获取锁超时
            ++timeout_count_;
            std::cout << "线程 " << thread_id << " 获取资源锁超时，操作: "
                      << operation << " 被取消" << std::endl;
            return false;
        }
    }

    // 使用超时锁和unique_lock
    bool processData(int thread_id, const std::string& data_name,
                     std::chrono::milliseconds timeout) {

        std::unique_lock<std::timed_mutex> lock(resource_mutex_,
                                               std::defer_lock);

        // 尝试在指定时间内获取锁
        if (lock.try_lock_for(timeout)) {
            ++access_count_;

            std::cout << "线程 " << thread_id << " 成功获取数据锁，处理: "
                      << data_name << std::endl;

            // 模拟数据处理
            std::this_thread::sleep_for(
                std::chrono::milliseconds(100));

            std::cout << "线程 " << thread_id << " 完成数据处理: "
                      << data_name << std::endl;

            // lock会在作用域结束时自动释放
            return true;
        } else {
            ++timeout_count_;
            std::cout << "线程 " << thread_id << " 获取数据锁超时，处理: "
                      << data_name << " 被跳过" << std::endl;
            return false;
        }
    }

    // 使用截止时间点的尝试锁定
    bool scheduleTask(int thread_id, const std::string& task_name,
                      std::chrono::system_clock::time_point deadline) {

        std::cout << "线程 " << thread_id << " 尝试调度任务 "
                  << task_name << "，截止时间: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(
                       deadline.time_since_epoch()).count() << "ms" << std::endl;

        // 尝试在截止时间前获取锁
        if (resource_mutex_.try_lock_until(deadline)) {
            ++access_count_;

            std::cout << "线程 " << thread_id << " 成功获取调度锁，执行任务: "
                      << task_name << std::endl;

            // 模拟任务执行
            std::this_thread::sleep_for(
                std::chrono::milliseconds(75));

            std::cout << "线程 " << thread_id << " 完成任务: "
                      << task_name << std::endl;

            resource_mutex_.unlock();
            return true;
        } else {
            ++timeout_count_;
            std::cout << "线程 " << thread_id << " 在截止时间前未能获取调度锁，"
                      << "任务: " << task_name << " 被取消" << std::endl;
            return false;
        }
    }

    // 获取统计信息
    void getStatistics() const {
        std::cout << "\n资源访问统计:\n";
        std::cout << "成功访问次数: " << access_count_ << std::endl;
        std::cout << "超时次数: " << timeout_count_ << std::endl;
        std::cout << "超时比例: "
                  << static_cast<double>(timeout_count_) /
                     (access_count_ + timeout_count_) * 100
                  << "%" << std::endl;
    }
};

int main() {
    ResourceManager resource_manager;
    std::vector<std::thread> threads;

    // 创建多个工作线程
    for (int i = 0; i < 10; ++i) {
        threads.push_back(std::thread([&resource_manager, i]() {
            // 模拟不同类型的资源访问

            // 情况1: 短超时
            resource_manager.useResource(
                i, "操作-" + std::to_string(i),
                std::chrono::milliseconds(200));

            // 让线程错开执行
            std::this_thread::sleep_for(std::chrono::milliseconds(20));

            // 情况2: 使用unique_lock的超时锁定
            resource_manager.processData(
                i, "数据-" + std::to_string(i),
                std::chrono::milliseconds(150));

            // 情况3: 使用截止时间点
            auto deadline = std::chrono::system_clock::now() +
                            std::chrono::milliseconds(180);
            resource_manager.scheduleTask(
                i, "任务-" + std::to_string(i), deadline);
        }));
    }

    // 等待所有线程完成
    for (auto& thread : threads) {
        thread.join();
    }

    // 显示统计信息
    resource_manager.getStatistics();

    return 0;
}
```

### 8.5 性能考虑

超时锁的性能特点：

1. **额外时间检查开销**：

   - 相比普通互斥锁，超时锁需要额外的时间检查
   - 在非竞争情况下，性能略低于普通互斥锁
2. **系统调用影响**：

   - 不同操作系统实现的性能特点不同
   - Windows和POSIX的超时锁实现可能有显著性能差异
3. **超时粒度**：

   - 操作系统提供的时间粒度可能限制超时精度
   - 在某些系统上，超短超时可能不准确
4. **系统负载影响**：

   - 系统负载高时，超时检测可能不如预期准确
   - 实际等待时间可能比请求的超时时间长

### 8.6 超时锁的应用场景

超时锁特别适合以下场景：

1. **死锁预防**：避免无限等待，为长时间锁定设置上限。
2. **响应式用户界面**：确保UI线程不会因锁等待而长时间阻塞。
3. **实时系统**：需要对资源访问时间有严格限制的系统。
4. **服务质量保证**：实现资源访问的超时策略，保证服务响应时间。
5. **故障恢复**：在分布式系统中，检测并从锁定线程失败中恢复。
6. **资源争用管理**：实现更灵活的资源竞争策略，如放弃并重试。

### 8.7 超时锁的最佳实践

1. **选择合适的超时值**：

   - 根据操作预期完成时间设置合理的超时
   - 考虑系统负载波动，留出足够的余量
   - 为不同类型的操作设置不同的超时值
2. **超时后的策略**：

   - 明确定义超时后的行为(重试、报错、降级)
   - 避免在超时处理中执行耗时操作
   - 考虑记录超时情况以便分析系统性能问题
3. **与条件变量配合**：

   - 对于条件等待，使用 `wait_for`或 `wait_until`
   - 注意条件检查的语义，避免虚假唤醒问题
4. **RAII与异常安全**：

   - 优先使用 `std::unique_lock`的超时方法
   - 确保即使发生异常也能正确释放锁
5. **避免嵌套超时锁**：

   - 嵌套锁定容易导致死锁和难以预测的行为
   - 如需多个锁，考虑使用 `std::scoped_lock`或一次性获取所有锁

### 8.8 超时锁的高级模式

1. **动态超时调整**：

   - 根据系统负载或历史数据动态调整超时时间
   - 实现自适应的资源争用管理
2. **超时重试模式**：

```cpp
bool perform_operation_with_retries(Data& data, int max_retries) {
    for (int attempt = 0; attempt < max_retries; ++attempt) {
        std::unique_lock<std::timed_mutex> lock(data.mutex, std::defer_lock);

        // 指数退避的超时时间
        auto timeout = std::chrono::milliseconds(100 * (1 << attempt));

        if (lock.try_lock_for(timeout)) {
            // 成功获取锁，执行操作
            process_data(data);
            return true;
        }

        // 锁定失败，记录并准备重试
        log_lock_timeout(attempt, timeout);
    }

    // 所有重试都失败
    return false;
}
```

3. **资源池管理**：

```cpp
template<typename Resource>
class TimeoutResourcePool {
private:
    std::vector<std::unique_ptr<Resource>> resources_;
    std::vector<bool> availability_;
    std::timed_mutex mutex_;

public:
    // 带超时的资源获取
    Resource* acquire(std::chrono::milliseconds timeout) {
        if (mutex_.try_lock_for(timeout)) {
            // 查找可用资源
            for (size_t i = 0; i < availability_.size(); ++i) {
                if (availability_[i]) {
                    availability_[i] = false;
                    mutex_.unlock();
                    return resources_[i].get();
                }
            }

            // 没有可用资源
            mutex_.unlock();
        }
        return nullptr;
    }

    // 资源释放
    void release(Resource* resource) {
        std::lock_guard<std::timed_mutex> lock(mutex_);
        for (size_t i = 0; i < resources_.size(); ++i) {
            if (resources_[i].get() == resource) {
                availability_[i] = true;
                break;
            }
        }
    }
};
```

4. **优先级锁定**：结合超时与尝试锁定实现优先级资源获取：

```cpp
enum class LockPriority { High, Medium, Low };

bool acquire_with_priority(std::timed_mutex& mutex, LockPriority priority) {
    // 根据优先级设置不同的超时策略
    switch (priority) {
        case LockPriority::High:
            // 高优先级：长时间尝试获取锁
            return mutex.try_lock_for(std::chrono::seconds(10));

        case LockPriority::Medium:
            // 中优先级：中等时间尝试
            return mutex.try_lock_for(std::chrono::seconds(5));

        case LockPriority::Low:
            // 低优先级：短时间尝试，或只尝试一次
            return mutex.try_lock_for(std::chrono::milliseconds(500));

        default:
            return false;
    }
}
```

### 8.9 与其他锁机制的组合

超时锁可以与其他锁机制结合，构建更复杂的同步策略：

1. **超时锁与条件变量**：

```cpp
std::timed_mutex mutex;
std::condition_variable_any cv;
bool ready = false;

// 等待条件，但有总体超时限制
bool wait_with_timeout(std::chrono::milliseconds timeout) {
    auto deadline = std::chrono::steady_clock::now() + timeout;

    std::unique_lock<std::timed_mutex> lock(mutex, std::defer_lock);
    if (!lock.try_lock_until(deadline)) {
        return false;  // 获取锁超时
    }

    // 计算剩余时间
    auto remaining = deadline - std::chrono::steady_clock::now();
    if (remaining <= std::chrono::milliseconds::zero()) {
        return false;  // 已经超时
    }

    // 等待条件或超时
    return cv.wait_for(lock, remaining, []{ return ready; });
}
```

2. **超时锁与共享锁**：

```cpp
std::shared_timed_mutex shared_mutex;

// 带超时的读取操作
bool read_with_timeout(Data& data, std::chrono::milliseconds timeout) {
    std::shared_lock<std::shared_timed_mutex> lock(shared_mutex, std::defer_lock);

    if (lock.try_lock_for(timeout)) {
        // 成功获取共享锁，执行读取操作
        read_data(data);
        return true;
    }

    return false;  // 获取锁超时
}

// 带超时的写入操作
bool write_with_timeout(Data& data, std::chrono::milliseconds timeout) {
    std::unique_lock<std::shared_timed_mutex> lock(shared_mutex, std::defer_lock);

    if (lock.try_lock_for(timeout)) {
        // 成功获取独占锁，执行写入操作
        write_data(data);
        return true;
    }

    return false;  // 获取锁超时
}
```

`<a id="共享锁"></a>`

## 9. 共享锁(Shared Lock)

### 9.1 基本概念

共享锁(Shared Lock)是一种RAII风格的读锁包装器，它封装了对读写锁(shared_mutex)的共享访问(读取模式)的管理。共享锁的主要特点是允许多个线程同时持有对同一共享互斥量的共享锁定，但与独占锁定互斥。

在C++中，共享锁是通过 `std::shared_lock`模板类实现的，它于C++14标准中引入，作为 `std::unique_lock`的共享锁定对应物。

共享锁的核心特性：

- 多个读取者可以同时持有共享锁
- 共享锁与独占锁(如 `std::unique_lock`)互斥
- 遵循RAII原则，自动管理锁的生命周期
- 支持条件变量及其他需要可锁定(Lockable)概念的上下文

### 9.2 与其他锁的关系

共享锁在锁家族中的位置：

1. **`std::unique_lock`**：独占锁，用于独占(写入)访问
2. **`std::shared_lock`**：共享锁，用于共享(读取)访问
3. **`std::lock_guard`**：简单的RAII独占锁包装器
4. **`std::scoped_lock`**：多锁获取的RAII包装器

锁的使用模式：

- 读取操作：使用 `std::shared_lock<std::shared_mutex>`
- 写入操作：使用 `std::unique_lock<std::shared_mutex>`

### 9.3 C++中的共享锁实现

`std::shared_lock`在C++14中引入，其接口与 `std::unique_lock`类似：

1. **构造函数**：

   - 默认构造：创建未关联任何互斥量的共享锁
   - 锁定构造：创建并锁定指定互斥量
   - 延迟锁定构造：创建但不立即锁定(使用 `std::defer_lock`)
   - 尝试锁定构造：创建并尝试锁定(使用 `std::try_to_lock`)
   - 采用所有权构造：接管已锁定的互斥量(使用 `std::adopt_lock`)
2. **锁操作**：

   - `lock()`：共享锁定互斥量
   - `try_lock()`：尝试共享锁定互斥量
   - `try_lock_for()`：尝试在指定时间内共享锁定
   - `try_lock_until()`：尝试在指定时间点前共享锁定
   - `unlock()`：解锁互斥量
3. **状态查询**：

   - `owns_lock()`：检查是否拥有锁
   - `operator bool()`：检查是否拥有锁
   - `mutex()`：获取关联的互斥量指针
4. **所有权管理**：

   - `release()`：释放互斥量的所有权而不解锁

### 9.4 详细示例

以下是一个使用共享锁的完整示例，展示了在读多写少场景中的应用：

```cpp
#include <shared_mutex>
#include <mutex>
#include <thread>
#include <vector>
#include <string>
#include <chrono>
#include <map>

class ThreadSafeDirectory {
private:
    std::shared_mutex mutex_;  // 读写锁
    std::map<std::string, std::string> entries_;  // 共享的目录数据

public:
    // 写操作：添加或更新条目(需要独占锁)
    bool add_or_update_entry(const std::string& key, const std::string& value) {
        std::unique_lock<std::shared_mutex> lock(mutex_);  // 获取独占(写)锁

        // 模拟一个耗时的写操作
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        entries_[key] = value;
        std::cout << "更新条目: [" << key << "] = " << value << std::endl;
        return true;
    }

    // 读操作：查找条目(只需要共享锁)
    std::string find_entry(const std::string& key) const {
        std::shared_lock<std::shared_mutex> lock(mutex_);  // 获取共享(读)锁

        // 模拟一个耗时的读操作
        std::this_thread::sleep_for(std::chrono::milliseconds(20));

        auto it = entries_.find(key);
        if (it == entries_.end()) {
            return "未找到";
        }

        std::cout << "读取条目: [" << key << "] = " << it->second << std::endl;
        return it->second;
    }

    // 另一个读操作：获取所有的键
    std::vector<std::string> get_all_keys() const {
        std::shared_lock<std::shared_mutex> lock(mutex_);  // 获取共享(读)锁

        std::vector<std::string> keys;
        for (const auto& entry : entries_) {
            keys.push_back(entry.first);
        }

        return keys;
    }

    // 写操作：删除条目(需要独占锁)
    bool delete_entry(const std::string& key) {
        std::unique_lock<std::shared_mutex> lock(mutex_);  // 获取独占(写)锁

        auto it = entries_.find(key);
        if (it == entries_.end()) {
            return false;
        }

        entries_.erase(it);
        std::cout << "删除条目: [" << key << "]" << std::endl;
        return true;
    }
};

int main() {
    ThreadSafeDirectory directory;

    // 预先添加一些条目
    directory.add_or_update_entry("one", "第一个");
    directory.add_or_update_entry("two", "第二个");
    directory.add_or_update_entry("three", "第三个");

    std::vector<std::thread> readers;
    std::vector<std::thread> writers;

    // 创建5个读线程，每个读线程读取多次
    for (int i = 0; i < 5; ++i) {
        readers.push_back(std::thread([&directory, i]() {
            for (int j = 0; j < 3; ++j) {
                directory.find_entry("one");
                directory.find_entry("two");
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }));
    }

    // 创建2个写线程
    for (int i = 0; i < 2; ++i) {
        writers.push_back(std::thread([&directory, i]() {
            directory.add_or_update_entry("four", "第四个-" + std::to_string(i));
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            directory.add_or_update_entry("five", "第五个-" + std::to_string(i));
        }));
    }

    // 等待所有线程完成
    for (auto& t : readers) {
        t.join();
    }
    for (auto& t : writers) {
        t.join();
    }

    // 验证最终结果
    auto keys = directory.get_all_keys();
    std::cout << "最终目录包含 " << keys.size() << " 个条目" << std::endl;
    for (const auto& key : keys) {
        std::cout << key << ": " << directory.find_entry(key) << std::endl;
    }

    return 0;
}
```

### 9.5 高级用例

以下是一些共享锁的高级用法：

1. **延迟锁定**：

```cpp
void delayed_lock_example(ThreadSafeDocument& doc) {
    // 创建未锁定的共享锁
    std::shared_lock<std::shared_mutex> lock(doc.get_mutex(), std::defer_lock);

    // 执行一些不需要锁的准备工作
    prepare_data();

    // 在需要时锁定
    lock.lock();

    // 执行受保护的操作
    process_document(doc);

    // 可以提前解锁
    lock.unlock();

    // 执行其他不需要锁的工作
    post_process();
}
```

2. **条件变量与共享锁**：

```cpp
std::shared_mutex mutex;
std::condition_variable_any cv;
bool data_ready = false;

void reader_thread() {
    std::shared_lock<std::shared_mutex> lock(mutex);

    // 等待数据准备好
    cv.wait(lock, []{ return data_ready; });

    // 数据现在可读
    read_data();
}

void writer_thread() {
    std::unique_lock<std::shared_mutex> lock(mutex);

    // 修改数据
    modify_data();

    // 标记数据已准备好
    data_ready = true;

    // 通知等待的读取者
    lock.unlock();  // 优化：在通知前解锁
    cv.notify_all();
}
```

3. **共享锁与作用域锁配合**：

```cpp
void process_with_multiple_locks(ResourceA& a, ResourceB& b) {
    // 同时获取两个资源的共享锁
    std::scoped_lock locks(
        std::shared_lock<std::shared_mutex>(a.mutex),
        std::shared_lock<std::shared_mutex>(b.mutex)
    );

    // 现在可以安全地读取两个资源
    process_resources(a, b);
}
```

4. **锁升级/降级模拟**：
   注意：C++标准库不直接支持锁升级，但可以模拟：

```cpp
class UpgradableSharedMutex {
private:
    std::shared_mutex shared_mutex_;
    std::mutex upgrade_mutex_;

public:
    // 获取共享锁
    void lock_shared() {
        shared_mutex_.lock_shared();
    }

    void unlock_shared() {
        shared_mutex_.unlock_shared();
    }

    // 获取独占锁
    void lock() {
        shared_mutex_.lock();
    }

    void unlock() {
        shared_mutex_.unlock();
    }

    // 尝试从共享锁升级到独占锁
    bool upgrade_lock() {
        // 首先获取升级互斥锁，确保一次只有一个线程尝试升级
        std::lock_guard<std::mutex> upgrade_guard(upgrade_mutex_);

        // 释放共享锁
        shared_mutex_.unlock_shared();

        // 获取独占锁
        shared_mutex_.lock();

        return true;
    }

    // 从独占锁降级到共享锁
    void downgrade_lock() {
        // 首先获取共享锁
        shared_mutex_.lock_shared();

        // 然后释放独占锁
        shared_mutex_.unlock();
    }
};
```

5. **手动控制锁的生命周期**：

```cpp
void manual_lock_lifecycle(std::shared_mutex& mutex, Data& data) {
    // 创建共享锁对象但不立即锁定
    std::shared_lock<std::shared_mutex> lock(mutex, std::defer_lock);

    try {
        // 第一阶段处理
        process_phase1(data);

        // 获取共享锁
        lock.lock();

        // 第二阶段处理(需要锁)
        process_phase2(data);

        // 解锁并继续
        lock.unlock();

        // 第三阶段处理
        process_phase3(data);

        // 再次锁定
        lock.lock();

        // 第四阶段处理(需要锁)
        process_phase4(data);

    } catch (const std::exception& e) {
        // 确保异常处理正确
        if (lock.owns_lock()) {
            lock.unlock();
        }
        handle_error(e);
    }
}
```

### 9.6 性能考虑

共享锁的性能特点：

1. **比独占锁更低的写入吞吐量**：

   - 写入操作需要等待所有读取操作完成
   - 高频写入场景下可能导致读取者饥饿
2. **读取密集场景的优势**：

   - 多个读取者可以并行访问，显著提高读取吞吐量
   - 读取操作间没有竞争，减少了缓存一致性流量
3. **开销来源**：

   - 锁状态检查
   - 读写锁内部计数器管理
   - 读写互斥处理逻辑
4. **RAII开销**：

   - RAII包装器本身有轻微的性能开销
   - 但这通常被异常安全和代码清晰度的好处抵消
5. **操作系统依赖**：

   - 不同操作系统和平台的共享锁实现性能特点不同
   - Linux的pthread读写锁和Windows的SRW锁有不同的性能特性

### 9.7 适用场景

共享锁最适合以下场景：

1. **读多写少的数据结构**：

   - 配置存储
   - 缓存系统
   - 查询密集型数据库
2. **多读者-单写者模式**：

   - 主要为读取优化的数据管理
   - 定期需要更新的共享数据
3. **数据可见性需求**：

   - 需要确保一组相关数据一致可见
   - 读取操作需要看到完整的更新结果
4. **规模性能要求**：

   - 需要在大量并发读取时保持性能
   - 写入延迟可以接受一定的增加

### 9.8 共享锁的限制

使用共享锁时需要注意以下限制：

1. **没有直接的锁升级支持**：

   - C++标准库不支持从共享锁直接升级到独占锁
   - 锁升级尝试可能导致死锁
2. **写入者可能饥饿**：

   - 在持续高读取负载下，写入者可能长时间无法获得锁
   - 需要额外机制确保写入者公平性
3. **递归支持限制**：

   - `std::shared_mutex`不支持递归锁定
   - 需要注意同一线程多次获取锁的情况
4. **性能取舍**：

   - 共享锁机制比简单互斥锁更复杂，有更多开销
   - 在读取不是主要负载的场景可能不值得使用
5. **缺乏优先级控制**：

   - 标准库实现不提供读者或写者优先级控制
   - 需要自定义实现特定的优先级策略

### 9.9 最佳实践

使用共享锁时的建议：

1. **锁的粒度**：

   - 尽可能减小锁的持有时间
   - 仅锁定需要保护的资源，避免过度锁定
2. **锁策略选择**：

   - 确认读取操作真的比写入更频繁
   - 监控锁竞争情况，必要时重新评估策略
3. **避免死锁**：

   - 保持一致的锁定顺序
   - 避免在持有锁时调用未知代码
   - 使用 `std::scoped_lock`同时获取多个锁
4. **代码清晰度**：

   - 明确标注共享和独占访问部分
   - 使用描述性的函数名区分读写操作
5. **锁升级/降级**：

   - 避免尝试升级锁(从共享到独占)
   - 如确实需要，先完全释放共享锁，再获取独占锁
   - 支持从独占锁降级到共享锁
6. **测试与验证**：

   - 使用并发测试工具验证正确性
   - 压力测试在各种读写负载比例下的性能

`<a id="乐观锁与悲观锁"></a>`

## 10. 乐观锁与悲观锁

### 10.1 基本概念

乐观锁和悲观锁是两种截然不同的并发控制思想，它们代表了处理并发冲突的两种哲学：

- **乐观锁(Optimistic Locking)**：假设冲突很少发生，先操作，发现冲突了再解决。
- **悲观锁(Pessimistic Locking)**：假设冲突经常发生，先获取锁，再操作。

这两种锁并不是特定的锁实现，而是两种锁策略，可以用多种方式实现。

### 10.2 乐观锁

#### 10.2.1 实现原理

乐观锁通常通过以下机制实现：

1. **版本号/时间戳**：

   - 每个数据项有一个版本号或时间戳
   - 读取时记录版本号
   - 写入前检查版本号是否变化
   - 版本号不变才执行写入并递增版本号
2. **CAS(Compare-And-Swap)**：

   - 原子地比较内存位置的值与预期值
   - 如果不同，则用新值替换
   - 如果相同，则操作失败
   - 通常会在循环中重试
3. **数据副本**：

   - 创建数据的本地副本
   - 修改副本
   - 尝试用副本替换原始数据
   - 如果原始数据已更改，则重试

#### 10.2.2 C++实现

C++中的乐观锁通常使用原子操作实现：

```cpp
#include <atomic>
#include <thread>
#include <iostream>

class OptimisticCounter {
private:
    std::atomic<int> value_{0};

public:
    void increment() {
        int expected = value_.load();
        while (!value_.compare_exchange_strong(expected, expected + 1)) {
            // expected 会被 compare_exchange_strong 更新为当前值
            // 循环直到成功
        }
    }

    int get() const {
        return value_.load();
    }
};

// 使用示例
int main() {
    OptimisticCounter counter;
    std::thread t1([&]{ for(int i=0; i<10000; ++i) counter.increment(); });
    std::thread t2([&]{ for(int i=0; i<10000; ++i) counter.increment(); });

    t1.join();
    t2.join();

    std::cout << "计数器值: " << counter.get() << std::endl;  // 应为20000
    return 0;
}
```

#### 10.2.3 优缺点

**优点**：

- 不需要互斥锁，避免上下文切换
- 在低冲突场景有更好的性能
- 没有死锁风险
- 支持更细粒度的并发

**缺点**：

- 在高冲突场景下性能下降
- 可能导致活锁(线程不断重试但总是失败)
- ABA问题
- 实现复杂，容易出错

### 10.3 悲观锁

#### 10.3.1 实现原理

悲观锁通常通过以下机制实现：

1. **互斥锁(Mutex)**：

   - 在访问共享资源前获取锁
   - 完成操作后释放锁
   - 确保同一时间只有一个线程访问资源
2. **信号量(Semaphore)**：

   - 限制同时访问资源的线程数
   - 可以用于实现多读单写机制
3. **监视器(Monitor)**：

   - 将数据和锁封装在一起
   - 提供条件变量进行线程间协调

#### 10.3.2 C++实现

C++中的悲观锁使用互斥锁系列实现：

```cpp
#include <mutex>
#include <thread>
#include <iostream>

class PessimisticCounter {
private:
    int value_{0};
    std::mutex mutex_;

public:
    void increment() {
        std::lock_guard<std::mutex> lock(mutex_);
        ++value_;
    }

    int get() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return value_;
    }
};

// 使用示例
int main() {
    PessimisticCounter counter;
    std::thread t1([&]{ for(int i=0; i<10000; ++i) counter.increment(); });
    std::thread t2([&]{ for(int i=0; i<10000; ++i) counter.increment(); });

    t1.join();
    t2.join();

    std::cout << "计数器值: " << counter.get() << std::endl;  // 应为20000
    return 0;
}
```

#### 10.3.3 优缺点

**优点**：

- 实现简单直观
- 冲突处理机制明确
- 在高冲突场景表现稳定
- 适合长时间持有锁的场景

**缺点**：

- 即使没有冲突也有锁开销
- 可能导致线程上下文切换
- 死锁风险
- 优先级反转风险
- 不利于提高并行程度

### 10.4 两种策略对比

以下是乐观锁和悲观锁的详细对比：

| 特性                 | 乐观锁               | 悲观锁               |
| -------------------- | -------------------- | -------------------- |
| **基本假设**   | 冲突很少发生         | 冲突经常发生         |
| **加锁时机**   | 在提交更改时检查冲突 | 在访问资源前获取锁   |
| **阻塞行为**   | 无阻塞(失败时重试)   | 阻塞等待锁释放       |
| **适用场景**   | 读多写少、冲突少     | 写操作频繁、冲突多   |
| **实现复杂度** | 较复杂               | 较简单               |
| **性能特点**   | 低冲突时高性能       | 高冲突时稳定性能     |
| **死锁风险**   | 无死锁风险           | 有死锁风险           |
| **资源消耗**   | 可能有CPU开销(重试)  | 可能有上下文切换开销 |
| **典型实现**   | CAS、版本控制        | 互斥锁、读写锁       |

### 10.5 实际应用

#### 10.5.1 乐观锁应用场景

1. **数据库系统**：

   - 多版本并发控制(MVCC)
   - 使用版本号或时间戳实现乐观事务
2. **缓存系统**：

   - CAS更新缓存值
   - 避免互斥锁影响读性能
3. **无锁数据结构**：

   - 无锁队列、栈
   - 基于原子操作实现
4. **低冲突高并发系统**：

   - 电子商务系统的库存更新(大多数时间只是查询)
   - 统计计数器

#### 10.5.2 悲观锁应用场景

1. **数据库系统**：

   - 行锁、表锁
   - 长事务中的资源访问
2. **文件系统**：

   - 文件锁
   - 目录修改保护
3. **共享资源池**：

   - 数据库连接池
   - 线程池管理
4. **高冲突系统**：

   - 多线程密集写入的日志系统
   - 状态频繁变更的资源

### 10.6 高级模式与变体

#### 10.6.1 混合策略

在实际系统中，经常结合两种策略使用：

```cpp
class HybridCounter {
private:
    std::atomic<int> value_{0};
    std::mutex mutex_;
    std::atomic<int> contention_counter_{0};

public:
    void increment() {
        // 先乐观尝试
        int expected = value_.load();
        for (int i = 0; i < 5; ++i) {
            if (value_.compare_exchange_strong(expected, expected + 1)) {
                return;  // 成功，直接返回
            }
            expected = value_.load();  // 更新期望值
        }

        // 乐观策略失败次数过多，切换为悲观策略
        ++contention_counter_;
        std::lock_guard<std::mutex> lock(mutex_);
        ++value_;
    }

    int get() const {
        return value_.load();
    }

    int get_contention() const {
        return contention_counter_.load();
    }
};
```

#### 10.6.2 自适应策略

根据冲突程度动态调整锁策略：

```cpp
class AdaptiveLocking {
private:
    std::atomic<int> value_{0};
    std::mutex mutex_;
    std::atomic<int> conflict_rate_{0};
    static constexpr int SAMPLE_WINDOW = 100;
    std::atomic<int> operations_{0};

public:
    void increment() {
        ++operations_;

        // 根据冲突率决定策略
        if (conflict_rate_.load() < 20 || operations_.load() % SAMPLE_WINDOW < 90) {
            // 低冲突或采样阶段：使用乐观锁
            int conflicts = 0;
            int expected = value_.load();
            while (!value_.compare_exchange_strong(expected, expected + 1)) {
                ++conflicts;
                if (conflicts >= 3) {
                    // 更新冲突统计
                    updateConflictRate(true);

                    // 切换到悲观锁
                    std::lock_guard<std::mutex> lock(mutex_);
                    ++value_;
                    return;
                }
                expected = value_.load();
            }

            // 成功，无冲突
            updateConflictRate(false);
        } else {
            // 高冲突：直接使用悲观锁
            std::lock_guard<std::mutex> lock(mutex_);
            ++value_;
        }
    }

private:
    void updateConflictRate(bool had_conflict) {
        if (operations_.load() % SAMPLE_WINDOW == 0) {
            // 采样窗口结束，重新计算冲突率
            conflict_rate_.store(0);
        }

        if (had_conflict) {
            // 简化的指数移动平均
            int old_rate = conflict_rate_.load();
            conflict_rate_.store(old_rate * 0.8 + 20);
        }
    }
};
```

### 10.7 最佳实践

1. **场景分析**：

   - 分析你的应用访问模式
   - 测量冲突率
   - 基于实际数据选择策略
2. **乐观锁使用建议**：

   - 设置合理的重试次数上限
   - 实现退避策略避免活锁
   - 解决ABA问题
   - 注意内存模型和内存顺序
3. **悲观锁使用建议**：

   - 最小化锁持有时间
   - 使用适当粒度的锁
   - 保持一致的锁获取顺序，避免死锁
   - 使用RAII风格锁管理
4. **混合策略建议**：

   - 对冲突频率不确定的场景，考虑自适应策略
   - 高并发场景可能需要更复杂的机制，如读写分离
   - 监控锁性能，根据实际情况优化
5. **测试**：

   - 执行压力测试评估两种策略在不同负载下的表现
   - 模拟各种并发场景，验证正确性
   - 关注边缘情况的处理

`<a id="读写意向锁"></a>`

## 11. 读写意向锁

### 11.1 基本概念

读写意向锁(Read-Write Intentions Lock)是一种特殊的读写锁，它允许多个线程同时持有读锁，但写操作仍然需要独占访问权限。读写意向锁通常用于以下场景：

1. 在读操作远多于写操作的场景中，读写意向锁可以显著提高并发性能。
2. 当读操作不需要互斥访问时，读写意向锁可以提高读取并发性。

### 11.2 底层实现原理

读写意向锁的实现通常基于读写锁：

1. **读写锁**：用于保护共享资源的访问。
2. **意向锁**：用于记录当前有多少线程持有读锁。

### 11.3 C++标准库中的读写意向锁

C++17引入了标准的读写意向锁实现：

1. **`std::shared_mutex`** - 完整的读写锁实现
2. **`std::shared_timed_mutex`**(C++14) - 支持超时的读写锁

读写意向锁提供以下操作：

对于写锁(独占访问)：

- `lock()` / `unlock()` - 获取/释放写锁
- `try_lock()` - 尝试获取写锁，立即返回

对于读锁(共享访问)：

- `lock_shared()` / `unlock_shared()` - 获取/释放读锁
- `try_lock_shared()` - 尝试获取读锁，立即返回

### 11.4 RAII锁包装器

C++标准库为读写意向锁提供了特殊的RAII包装器：

1. **`std::unique_lock`** - 用于独占(写)锁
2. **`std::shared_lock`**(C++14) - 用于共享(读)锁

这些包装器确保锁的正确获取和释放，遵循RAII原则。

### 11.5 详细示例

以下是一个使用读写意向锁的完整示例，展示了在读多写少场景中的应用：

```cpp
#include <shared_mutex>
#include <mutex>
#include <thread>
#include <vector>
#include <iostream>
#include <string>
#include <chrono>
#include <atomic>
#include <iomanip>

class ThreadSafeDocument {
private:
    mutable std::shared_mutex mutex_;  // 可共享的互斥锁
    std::string content_;               // 文档内容
    std::string title_;                 // 文档标题
    std::atomic<int> reader_count_{0};  // 当前读取者计数
    std::atomic<int> writer_count_{0};  // 当前写入者计数

public:
    ThreadSafeDocument(const std::string& title = "", const std::string& content = "")
        : title_(title), content_(content) {}

    // 读取文档内容 - 使用共享锁
    std::string read(int reader_id) const {
        // 使用RAII方式获取共享锁
        std::shared_lock<std::shared_mutex> lock(mutex_);

        // 增加读取者计数
        int current_readers = ++reader_count_;

        std::cout << "读者 #" << reader_id << " 开始读取文档'"
                  << title_ << "' (当前读者: " << current_readers
                  << ", 写入者: " << writer_count_ << ")" << std::endl;

        // 模拟阅读过程
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        std::string result = "标题: " + title_ + "\n内容: " + content_;

        // 减少读取者计数
        --reader_count_;

        std::cout << "读者 #" << reader_id << " 完成读取文档'"
                  << title_ << "'" << std::endl;

        return result;
    }

    // 写入文档内容 - 使用独占锁
    void write(int writer_id, const std::string& new_content) {
        // 使用RAII方式获取独占锁
        std::unique_lock<std::shared_mutex> lock(mutex_);

        // 增加写入者计数
        int current_writers = ++writer_count_;

        std::cout << "写入者 #" << writer_id << " 开始修改文档'"
                  << title_ << "' (当前读者: " << reader_count_
                  << ", 写入者: " << current_writers << ")" << std::endl;

        // 模拟写入过程
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        // 更新文档内容
        content_ = new_content;

        // 减少写入者计数
        --writer_count_;

        std::cout << "写入者 #" << writer_id << " 完成修改文档'"
                  << title_ << "'" << std::endl;
    }

    // 更新文档标题 - 使用独占锁
    void updateTitle(int writer_id, const std::string& new_title) {
        // 使用RAII方式获取独占锁
        std::unique_lock<std::shared_mutex> lock(mutex_);

        // 增加写入者计数
        int current_writers = ++writer_count_;

        std::cout << "写入者 #" << writer_id << " 开始更新标题从 '"
                  << title_ << "' 到 '" << new_title
                  << "' (当前读者: " << reader_count_
                  << ", 写入者: " << current_writers << ")" << std::endl;

        // 模拟更新过程
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        // 更新标题
        title_ = new_title;

        // 减少写入者计数
        --writer_count_;

        std::cout << "写入者 #" << writer_id << " 完成标题更新为 '"
                  << title_ << "'" << std::endl;
    }

    // 快速查询标题 - 使用共享锁但不睡眠
    std::string getTitle() const {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        return title_;
    }

    // 演示锁定状态转移
    void demonstrateLockTransfer(int thread_id) {
        std::shared_lock<std::shared_mutex> lock(mutex_);

        std::cout << "线程 #" << thread_id << " 获取了共享锁，"
                  << "准备转移所有权" << std::endl;

        // 转移锁的所有权到另一个函数
        processWith(thread_id, std::move(lock));

        std::cout << "线程 #" << thread_id << " 现在锁的状态: "
                  << (lock.owns_lock() ? "仍然拥有锁" : "不再拥有锁")
                  << std::endl;
    }

private:
    // 接受锁的所有权并使用它
    void processWith(int thread_id, std::shared_lock<std::shared_mutex> lock) {
        // 检查我们是否真的获得了锁
        if (lock.owns_lock()) {
            std::cout << "处理函数获得了线程 #" << thread_id
                      << " 转移的锁" << std::endl;

            // 使用受保护的资源
            std::cout << "安全访问标题: " << title_ << std::endl;

            // 锁会在函数结束时自动释放
        }
    }
};

// 演示各种读写意向锁使用模式
int main() {
    // 创建一个共享文档
    ThreadSafeDocument doc("初始标题", "这是文档的初始内容。");

    // 准备线程
    std::vector<std::thread> threads;

    // 创建多个读取线程
    for (int i = 0; i < 8; ++i) {
        threads.push_back(std::thread([&doc, i]() {
            // 每个线程读取几次文档
            for (int j = 0; j < 3; ++j) {
                std::string content = doc.read(i);

                // 线程间隔
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        }));
    }

    // 创建几个写入线程
    for (int i = 0; i < 3; ++i) {
        threads.push_back(std::thread([&doc, i]() {
            // 更新文档内容
            doc.write(i, "这是写入者 #" + std::to_string(i) + " 修改的新内容。");

            // 线程间隔
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            // 更新文档标题
            doc.updateTitle(i, "写入者 #" + std::to_string(i) + " 的标题");
        }));
    }

    // 演示锁转移
    threads.push_back(std::thread([&doc]() {
        // 等待其他线程先工作一会
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        // 演示锁转移
        doc.demonstrateLockTransfer(999);
    }));

    // 等待所有线程完成
    for (auto& t : threads) {
        t.join();
    }

    // 最终状态
    std::cout << "\n最终文档状态:\n"
              << "标题: " << doc.getTitle() << std::endl;
    std::cout << doc.read(-1) << std::endl;

    return 0;
}
```

### 11.6 高级用例

以下是一些读写意向锁的高级用法：

1. **延迟锁定**：

```cpp
void delayed_lock_example(ThreadSafeDocument& doc) {
    // 创建未锁定的读写意向锁
    std::shared_lock<std::shared_mutex> read_lock(doc.get_mutex(), std::defer_lock);
    std::unique_lock<std::shared_mutex> write_lock(doc.get_mutex(), std::defer_lock);

    // 执行一些不需要锁的准备工作
    prepare_data();

    // 在需要时锁定
    read_lock.lock();
    write_lock.lock();

    // 执行受保护的操作
    process_document(doc);

    // 可以提前解锁
    read_lock.unlock();
    write_lock.unlock();

    // 执行其他不需要锁的工作
    post_process();
}
```

2. **条件变量与读写意向锁**：

```cpp
std::shared_mutex mutex;
std::condition_variable_any cv;
bool data_ready = false;

void reader_thread() {
    std::shared_lock<std::shared_mutex> read_lock(mutex);

    // 等待数据准备好
    cv.wait(read_lock, []{ return data_ready; });

    // 数据现在可读
    read_data();
}

void writer_thread() {
    std::unique_lock<std::shared_mutex> write_lock(mutex);

    // 修改数据
    modify_data();

    // 标记数据已准备好
    data_ready = true;

    // 通知等待的读取者
    write_lock.unlock();  // 优化：在通知前解锁
    cv.notify_all();
}
```

3. **读写意向锁与作用域锁配合**：

```cpp
void process_with_multiple_locks(ResourceA& a, ResourceB& b) {
    // 同时获取两个资源的读写意向锁
    std::scoped_lock locks(
        std::shared_lock<std::shared_mutex>(a.read_mutex),
        std::unique_lock<std::shared_mutex>(b.write_mutex)
    );

    // 现在可以安全地读取两个资源
    process_resources(a, b);
}
```

4. **锁升级/降级模拟**：
   注意：C++标准库不直接支持锁升级，但可以模拟：

```cpp
class UpgradableSharedMutex {
private:
    std::shared_mutex shared_mutex_;
    std::mutex upgrade_mutex_;

public:
    // 获取共享锁
    void lock_shared() {
        shared_mutex_.lock_shared();
    }

    void unlock_shared() {
        shared_mutex_.unlock_shared();
    }

    // 获取独占锁
    void lock() {
        shared_mutex_.lock();
    }

    void unlock() {
        shared_mutex_.unlock();
    }

    // 尝试从共享锁升级到独占锁
    bool upgrade_lock() {
        // 首先获取升级互斥锁，确保一次只有一个线程尝试升级
        std::lock_guard<std::mutex> upgrade_guard(upgrade_mutex_);

        // 释放共享锁
        shared_mutex_.unlock_shared();

        // 获取独占锁
        shared_mutex_.lock();

        return true;
    }

    // 从独占锁降级到共享锁
    void downgrade_lock() {
        // 首先获取共享锁
        shared_mutex_.lock_shared();

        // 然后释放独占锁
        shared_mutex_.unlock();
    }
};
```

5. **手动控制锁的生命周期**：

```cpp
void manual_lock_lifecycle(std::shared_mutex& mutex, Data& data) {
    // 创建读写意向锁对象但不立即锁定
    std::shared_lock<std::shared_mutex> read_lock(mutex, std::defer_lock);
    std::unique_lock<std::shared_mutex> write_lock(mutex, std::defer_lock);

    try {
        // 第一阶段处理
        process_phase1(data);

        // 获取读写意向锁
        read_lock.lock();
        write_lock.lock();

        // 第二阶段处理(需要锁)
        process_phase2(data);

        // 解锁并继续
        read_lock.unlock();
        write_lock.unlock();

        // 第三阶段处理
        process_phase3(data);

        // 再次锁定
        read_lock.lock();
        write_lock.lock();

        // 第四阶段处理(需要锁)
        process_phase4(data);

    } catch (const std::exception& e) {
        // 确保异常处理正确
        if (read_lock.owns_lock()) {
            read_lock.unlock();
        }
        if (write_lock.owns_lock()) {
            write_lock.unlock();
        }
        handle_error(e);
    }
}
```

### 11.7 性能考虑

读写意向锁的性能特点：

1. **比独占锁更低的写入吞吐量**：

   - 写入操作需要等待所有读取操作完成
   - 高频写入场景下可能导致读取者饥饿
2. **读取密集场景的优势**：

   - 多个读取者可以并行访问，显著提高读取吞吐量
   - 读取操作间没有竞争，减少了缓存一致性流量
3. **开销来源**：

   - 锁状态检查
   - 读写锁内部计数器管理
   - 读写互斥处理逻辑
4. **RAII开销**：

   - RAII包装器本身有轻微的性能开销
   - 但这通常被异常安全和代码清晰度的好处抵消
5. **操作系统依赖**：

   - 不同操作系统和平台的读写意向锁实现性能特点不同
   - Linux的pthread读写锁和Windows的SRW锁有不同的性能特性
6. **内存开销**：读写意向锁通常比普通互斥锁需要更多的内存来存储额外状态。

### 11.8 适用场景

读写意向锁最适合以下场景：

1. **读多写少的访问模式**：当读操作远多于写操作时，读写意向锁可以显著提高并发性能。
2. **读操作耗时**：当读操作需要相对较长的时间时，允许并发读取可以提高整体吞吐量。
3. **写操作可以容忍一定延迟**：写操作可能需要等待所有读操作完成。
4. **需要数据一致性**：相比无锁算法，读写意向锁确保了数据的一致性。
5. **缓存、配置存储、数据库等系统**：这些系统通常具有读多写少的访问模式。

### 11.9 读写意向锁的限制

使用读写意向锁时需要注意以下限制：

1. **没有直接的锁升级支持**：

   - C++标准库不支持从读写意向锁直接升级到独占锁
   - 锁升级尝试可能导致死锁
2. **写入者可能饥饿**：

   - 在持续高读取负载下，写入者可能长时间无法获得锁
   - 需要额外机制确保写入者公平性
3. **递归支持限制**：

   - `std::shared_mutex`不支持递归锁定
   - 需要注意同一线程多次获取锁的情况
4. **性能取舍**：

   - 读写意向锁机制比简单互斥锁更复杂，有更多开销
   - 在读取不是主要负载的场景可能不值得使用
5. **缺乏优先级控制**：

   - 标准库实现不提供读者或写者优先级控制
   - 需要自定义实现特定的优先级策略

### 11.10 最佳实践

使用读写意向锁时的建议：

1. **锁的粒度**：

   - 尽可能减小锁的持有时间
   - 仅锁定需要保护的资源，避免过度锁定
2. **锁策略选择**：

   - 确认读取操作真的比写入更频繁
   - 监控锁竞争情况，必要时重新评估策略
3. **避免死锁**：

   - 保持一致的锁定顺序
   - 避免在持有锁时调用未知代码
   - 使用 `std::scoped_lock`同时获取多个锁
4. **代码清晰度**：

   - 明确标注共享和独占访问部分
   - 使用描述性的函数名区分读写操作
5. **锁升级/降级**：

   - 避免尝试升级锁(从读写意向到独占)
   - 如确实需要，先完全释放读写意向锁，再获取独占锁
   - 支持从独占锁降级到读写意向锁
6. **测试与验证**：

   - 使用并发测试工具验证正确性
   - 压力测试在各种读写负载比例下的性能

`<a id="死锁问题"></a>`

## 12. 死锁问题与解决方案

### 12.1 基本概念

死锁(Deadlock)是指两个或多个线程在等待对方释放锁的情况下无法继续执行的情况。死锁通常是由于以下原因之一引起的：

1. **循环等待**：线程A等待线程B持有的锁，而线程B又在等待线程A持有的锁。
2. **互斥条件**：线程在持有锁的情况下请求其他锁。
3. **不可剥夺条件**：线程在持有锁的情况下无法被强制释放锁。
4. **请求和保持条件**：线程在持有锁的情况下请求其他锁。

### 12.2 死锁的检测与避免

死锁的检测与避免通常需要以下步骤：

1. **检测死锁**：使用工具或算法检测系统中是否存在死锁。
2. **避免死锁**：通过设计合理的锁顺序、使用超时锁或信号量等机制来避免死锁。
3. **恢复死锁**：在检测到死锁后，采取措施恢复系统状态。

### 12.3 死锁的解决方案

1. **使用超时锁**：超时锁可以避免线程长时间等待锁，从而减少死锁的可能性。
2. **使用信号量**：信号量可以控制线程对资源的访问顺序，从而避免死锁。
3. **使用条件变量**：条件变量可以协调线程之间的同步，从而避免死锁。
4. **使用共享锁**：共享锁可以允许多个线程同时访问共享资源，从而减少死锁的可能性。
5. **使用读写意向锁**：读写意向锁可以允许多个线程同时访问共享资源，从而减少死锁的可能性。

### 12.4 详细示例

以下是一个使用超时锁和信号量避免死锁的示例：

```cpp
#include <mutex>
#include <thread>
#include <iostream>
#include <vector>
#include <chrono>
#include <semaphore>

class ResourceManager {
private:
    std::timed_mutex resource_mutex_;
    std::counting_semaphore<> available_connections_;
    std::atomic<int> access_count_{0};
    std::atomic<int> timeout_count_{0};

public:
    ResourceManager(int pool_size) : available_connections_(pool_size) {}

    // 借用一个连接(可能阻塞)
    bool borrow() {
        // 等待有可用连接
        if (!available_connections_.try_acquire()) {
            return false;  // 没有可用连接
        }

        // 获取一个可用连接的索引
        std::lock_guard<std::timed_mutex> lock(resource_mutex_);
        int idx = available_connections_.acquire();

        std::cout << "借出连接 #" << (idx + 1) << ", 剩余可用: "
                  << available_connections_.value() << std::endl;

        return true;
    }

    // 归还连接
    void return_connection() {
        std::lock_guard<std::timed_mutex> lock(resource_mutex_);

        // 查找连接在池中的索引
        for (int i = 0; i < available_connections_.value(); ++i) {
            if (available_connections_[i]) {
                available_connections_[i] = false;
                std::cout << "归还连接 #" << (i + 1) << ", 现在可用: "
                          << available_connections_.value() << std::endl;
                break;
            }
        }

        // 增加可用连接计数
        available_connections_.release();
    }

    // 使用超时锁访问资源
    bool useResource(int thread_id, const std::string& operation,
                     std::chrono::milliseconds timeout) {

        // 尝试在指定时间内获取锁
        if (resource_mutex_.try_lock_for(timeout)) {
            // 获取锁成功
            ++access_count_;

            std::cout << "线程 " << thread_id << " 成功获取资源锁，执行: "
                      << operation << std::endl;

            // 模拟工作负载 - 根据线程ID变化工作时间
            std::this_thread::sleep_for(
                std::chrono::milliseconds(50 + thread_id * 20));

            std::cout << "线程 " << thread_id << " 完成操作: "
                      << operation << std::endl;

            // 释放锁
            resource_mutex_.unlock();
            return true;
        } else {
            // 获取锁超时
            ++timeout_count_;
            std::cout << "线程 " << thread_id << " 获取资源锁超时，操作: "
                      << operation << " 被取消" << std::endl;
            return false;
        }
    }

    // 使用超时锁和unique_lock
    bool processData(int thread_id, const std::string& data_name,
                     std::chrono::milliseconds timeout) {

        std::unique_lock<std::timed_mutex> lock(resource_mutex_,
                                               std::defer_lock);

        // 尝试在指定时间内获取锁
        if (lock.try_lock_for(timeout)) {
            ++access_count_;

            std::cout << "线程 " << thread_id << " 成功获取数据锁，处理: "
                      << data_name << std::endl;

            // 模拟数据处理
            std::this_thread::sleep_for(
                std::chrono::milliseconds(100));

            std::cout << "线程 " << thread_id << " 完成数据处理: "
                      << data_name << std::endl;

            // lock会在作用域结束时自动释放
            return true;
        } else {
            ++timeout_count_;
            std::cout << "线程 " << thread_id << " 获取数据锁超时，处理: "
                      << data_name << " 被跳过" << std::endl;
            return false;
        }
    }

    // 使用截止时间点的尝试锁定
    bool scheduleTask(int thread_id, const std::string& task_name,
                      std::chrono::system_clock::time_point deadline) {

        std::cout << "线程 " << thread_id << " 尝试调度任务 "
                  << task_name << "，截止时间: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(
                       deadline.time_since_epoch()).count() << "ms" << std::endl;

        // 尝试在截止时间前获取锁
        if (resource_mutex_.try_lock_until(deadline)) {
            ++access_count_;

            std::cout << "线程 " << thread_id << " 成功获取调度锁，执行任务: "
                      << task_name << std::endl;

            // 模拟任务执行
            std::this_thread::sleep_for(
                std::chrono::milliseconds(75));

            std::cout << "线程 " << thread_id << " 完成任务: "
                      << task_name << std::endl;

            resource_mutex_.unlock();
            return true;
        } else {
            ++timeout_count_;
            std::cout << "线程 " << thread_id << " 在截止时间前未能获取调度锁，"
                      << "任务: " << task_name << " 被取消" << std::endl;
            return false;
        }
    }

    // 获取统计信息
    void getStatistics() const {
        std::cout << "\n资源访问统计:\n";
        std::cout << "成功访问次数: " << access_count_ << std::endl;
        std::cout << "超时次数: " << timeout_count_ << std::endl;
        std::cout << "超时比例: "
                  << static_cast<double>(timeout_count_) /
                     (access_count_ + timeout_count_) * 100
                  << "%" << std::endl;
    }
};

int main() {
    ResourceManager resource_manager(5);
    std::vector<std::thread> threads;

    // 创建多个工作线程
    for (int i = 0; i < 10; ++i) {
        threads.push_back(std::thread([&resource_manager, i]() {
            // 模拟不同类型的资源访问

            // 情况1: 短超时
            resource_manager.useResource(
                i, "操作-" + std::to_string(i),
                std::chrono::milliseconds(200));

            // 让线程错开执行
            std::this_thread::sleep_for(std::chrono::milliseconds(20));

            // 情况2: 使用unique_lock的超时锁定
            resource_manager.processData(
                i, "数据-" + std::to_string(i),
                std::chrono::milliseconds(150));

            // 情况3: 使用截止时间点
            auto deadline = std::chrono::system_clock::now() +
                            std::chrono::milliseconds(180);
            resource_manager.scheduleTask(
                i, "任务-" + std::to_string(i), deadline);
        }));
    }

    // 等待所有线程完成
    for (auto& thread : threads) {
        thread.join();
    }

    // 显示统计信息
    resource_manager.getStatistics();

    return 0;
}
```

### 12.4 死锁的检测与避免

死锁的检测与避免通常需要以下步骤：

1. **检测死锁**：使用工具或算法检测系统中是否存在死锁。
2. **避免死锁**：通过设计合理的锁顺序、使用超时锁或信号量等机制来避免死锁。
3. **恢复死锁**：在检测到死锁后，采取措施恢复系统状态。

### 12.5 死锁的解决方案

1. **使用超时锁**：超时锁可以避免线程长时间等待锁，从而减少死锁的可能性。
2. **使用信号量**：信号量可以控制线程对资源的访问顺序，从而避免死锁。
3. **使用条件变量**：条件变量可以协调线程之间的同步，从而避免死锁。
4. **使用共享锁**：共享锁可以允许多个线程同时访问共享资源，从而减少死锁的可能性。
5. **使用读写意向锁**：读写意向锁可以允许多个线程同时访问共享资源，从而减少死锁的可能性。

### 12.6 详细示例

以下是一个使用超时锁和信号量避免死锁的示例：

```cpp
#include <mutex>
#include <thread>
#include <iostream>
#include <vector>
#include <chrono>
#include <semaphore>

class ResourceManager {
private:
    std::timed_mutex resource_mutex_;
    std::counting_semaphore<> available_connections_;
    std::atomic<int> access_count_{0};
    std::atomic<int> timeout_count_{0};

public:
    ResourceManager(int pool_size) : available_connections_(pool_size) {}

    // 借用一个连接(可能阻塞)
    bool borrow() {
        // 等待有可用连接
        if (!available_connections_.try_acquire()) {
            return false;  // 没有可用连接
        }

        // 获取一个可用连接的索引
        std::lock_guard<std::timed_mutex> lock(resource_mutex_);
        int idx = available_connections_.acquire();

        std::cout << "借出连接 #" << (idx + 1) << ", 剩余可用: "
                  << available_connections_.value() << std::endl;

        return true;
    }

    // 归还连接
    void return_connection() {
        std::lock_guard<std::timed_mutex> lock(resource_mutex_);

        // 查找连接在池中的索引
        for (int i = 0; i < available_connections_.value(); ++i) {
            if (available_connections_[i]) {
                available_connections_[i] = false;
                std::cout << "归还连接 #" << (i + 1) << ", 现在可用: "
                          << available_connections_.value() << std::endl;
                break;
            }
        }

        // 增加可用连接计数
        available_connections_.release();
    }

    // 使用超时锁访问资源
    bool useResource(int thread_id, const std::string& operation,
                     std::chrono::milliseconds timeout) {

        // 尝试在指定时间内获取锁
        if (resource_mutex_.try_lock_for(timeout)) {
            // 获取锁成功
            ++access_count_;

            std::cout << "线程 " << thread_id << " 成功获取资源锁，执行: "
                      << operation << std::endl;

            // 模拟工作负载 - 根据线程ID变化工作时间
            std::this_thread::sleep_for(
                std::chrono::milliseconds(50 + thread_id * 20));

            std::cout << "线程 " << thread_id << " 完成操作: "
                      << operation << std::endl;

            // 释放锁
            resource_mutex_.unlock();
            return true;
        } else {
            // 获取锁超时
            ++timeout_count_;
            std::cout << "线程 " << thread_id << " 获取资源锁超时，操作: "
                      << operation << " 被取消" << std::endl;
            return false;
        }
    }

    // 使用超时锁和unique_lock
    bool processData(int thread_id, const std::string& data_name,
                     std::chrono::milliseconds timeout) {

        std::unique_lock<std::timed_mutex> lock(resource_mutex_,
                                               std::defer_lock);

        // 尝试在指定时间内获取锁
        if (lock.try_lock_for(timeout)) {
            ++access_count_;

            std::cout << "线程 " << thread_id << " 成功获取数据锁，处理: "
                      << data_name << std::endl;

            // 模拟数据处理
            std::this_thread::sleep_for(
                std::chrono::milliseconds(100));

            std::cout << "线程 " << thread_id << " 完成数据处理: "
                      << data_name << std::endl;

            // lock会在作用域结束时自动释放
            return true;
        } else {
            ++timeout_count_;
            std::cout << "线程 " << thread_id << " 获取数据锁超时，处理: "
                      << data_name << " 被跳过" << std::endl;
            return false;
        }
    }

    // 使用截止时间点的尝试锁定
    bool scheduleTask(int thread_id, const std::string& task_name,
                      std::chrono::system_clock::time_point deadline) {

        std::cout << "线程 " << thread_id << " 尝试调度任务 "
                  << task_name << "，截止时间: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(
                       deadline.time_since_epoch()).count() << "ms" << std::endl;

        // 尝试在截止时间前获取锁
        if (resource_mutex_.try_lock_until(deadline)) {
            ++access_count_;

            std::cout << "线程 " << thread_id << " 成功获取调度锁，执行任务: "
                      << task_name << std::endl;

            // 模拟任务执行
            std::this_thread::sleep_for(
                std::chrono::milliseconds(75));

            std::cout << "线程 " << thread_id << " 完成任务: "
                      << task_name << std::endl;

            resource_mutex_.unlock();
            return true;
        } else {
            ++timeout_count_;
            std::cout << "线程 " << thread_id << " 在截止时间前未能获取调度锁，"
                      << "任务: " << task_name << " 被取消" << std::endl;
            return false;
        }
    }

    // 获取统计信息
    void getStatistics() const {
        std::cout << "\n资源访问统计:\n";
        std::cout << "成功访问次数: " << access_count_ << std::endl;
        std::cout << "超时次数: " << timeout_count_ << std::endl;
        std::cout << "超时比例: "
                  << static_cast<double>(timeout_count_) /
                     (access_count_ + timeout_count_) * 100
                  << "%" << std::endl;
    }
};

int main() {
    ResourceManager resource_manager(5);
    std::vector<std::thread> threads;

    // 创建多个工作线程
    for (int i = 0; i < 10; ++i) {
        threads.push_back(std::thread([&resource_manager, i]() {
            // 模拟不同类型的资源访问

            // 情况1: 短超时
            resource_manager.useResource(
                i, "操作-" + std::to_string(i),
                std::chrono::milliseconds(200));

            // 让线程错开执行
            std::this_thread::sleep_for(std::chrono::milliseconds(20));

            // 情况2: 使用unique_lock的超时锁定
            resource_manager.processData(
                i, "数据-" + std::to_string(i),
                std::chrono::milliseconds(150));

            // 情况3: 使用截止时间点
            auto deadline = std::chrono::system_clock::now() +
                            std::chrono::milliseconds(180);
            resource_manager.scheduleTask(
                i, "任务-" + std::to_string(i), deadline);
        }));
    }

    // 等待所有线程完成
    for (auto& thread : threads) {
        thread.join();
    }

    // 显示统计信息
    resource_manager.getStatistics();

    return 0;
}
```

### 12.4 死锁的检测与避免

死锁的检测与避免通常需要以下步骤：

1. **检测死锁**：使用工具或算法检测系统中是否存在死锁。
2. **避免死锁**：通过设计合理的锁顺序、使用超时锁或信号量等机制来避免死锁。
3. **恢复死锁**：在检测到死锁后，采取措施恢复系统状态。

### 12.5 死锁的解决方案

1. **使用超时锁**：超时锁可以避免线程长时间等待锁，从而减少死锁的可能性。
2. **使用信号量**：信号量可以控制线程对资源的访问顺序，从而避免死锁。
3. **使用条件变量**：条件变量可以协调线程之间的同步，从而避免死锁。
4. **使用共享锁**：共享锁可以允许多个线程同时访问共享资源，从而减少死锁的可能性。
5. **使用读写意向锁**：读写意向锁可以允许多个线程同时访问共享资源，从而减少死锁的可能性。

### 12.6 详细示例

以下是一个使用超时锁和信号量避免死锁的示例：

```cpp
#include <mutex>
#include <thread>
#include <iostream>
#include <vector>
#include <chrono>
#include <semaphore>

class ResourceManager {
private:
    std::timed_mutex resource_mutex_;
    std::counting_semaphore<> available_connections_;
    std::atomic<int> access_count_{0};
    std::atomic<int> timeout_count_{0};

public:
    ResourceManager(int pool_size) : available_connections_(pool_size) {}

    // 借用一个连接(可能阻塞)
    bool borrow() {
        // 等待有可用连接
        if (!available_connections_.try_acquire()) {
            return false;  // 没有可用连接
        }

        // 获取一个可用连接的索引
        std::lock_guard<std::timed_mutex> lock(resource_mutex_);
        int idx = available_connections_.acquire();

        std::cout << "借出连接 #" << (idx + 1) << ", 剩余可用: "
                  << available_connections_.value() << std::endl;

        return true;
    }

    // 归还连接
    void return_connection() {
        std::lock_guard<std::timed_mutex> lock(resource_mutex_);

        // 查找连接在池中的索引
        for (int i = 0; i < available_connections_.value(); ++i) {
            if (available_connections_[i]) {
                available_connections_[i] = false;
                std::cout << "归还连接 #" << (i + 1) << ", 现在可用: "
                          << available_connections_.value() << std::endl;
                break;
            }
        }

        // 增加可用连接计数
        available_connections_.release();
    }

    // 使用超时锁访问资源
    bool useResource(int thread_id, const std::string& operation,
                     std::chrono::milliseconds timeout) {

        // 尝试在指定时间内获取锁
        if (resource_mutex_.try_lock_for(timeout)) {
            // 获取锁成功
            ++access_count_;

            std::cout << "线程 " << thread_id << " 成功获取资源锁，执行: "
                      << operation << std::endl;

            // 模拟工作负载 - 根据线程ID变化工作时间
            std::this_thread::sleep_for(
                std::chrono::milliseconds(50 + thread_id * 20));

            std::cout << "线程 " << thread_id << " 完成操作: "
                      << operation << std::endl;

            // 释放锁
            resource_mutex_.unlock();
            return true;
        } else {
            // 获取锁超时
            ++timeout_count_;
            std::cout << "线程 " << thread_id << " 获取资源锁超时，操作: "
                      << operation << " 被取消" << std::endl;
            return false;
        }
    }

    // 使用超时锁和unique_lock
    bool processData(int thread_id, const std::string& data_name,
                     std::chrono::milliseconds timeout) {

        std::unique_lock<std::timed_mutex> lock(resource_mutex_,
                                               std::defer_lock);

        // 尝试在指定时间内获取锁
        if (lock.try_lock_for(timeout)) {
            ++access_count_;

            std::cout << "线程 " << thread_id << " 成功获取数据锁，处理: "
                      << data_name << std::endl;

            // 模拟数据处理
            std::this_thread::sleep_for(
                std::chrono::milliseconds(100));

            std::cout << "线程 " << thread_id << " 完成数据处理: "
                      << data_name << std::endl;

            // lock会在作用域结束时自动释放
            return true;
        } else {
            ++timeout_count_;
            std::cout << "线程 " << thread_id << " 获取数据锁超时，处理: "
                      << data_name << " 被跳过" << std::endl;
            return false;
        }
    }

    // 使用截止时间点的尝试锁定
    bool scheduleTask(int thread_id, const std::string& task_name,
                      std::chrono::system_clock::time_point deadline) {

        std::cout << "线程 " << thread_id << " 尝试调度任务 "
                  << task_name << "，截止时间: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(
                       deadline.time_since_epoch()).count() << "ms" << std::endl;

        // 尝试在截止时间前获取锁
        if (resource_mutex_.try_lock_until(deadline)) {
            ++access_count_;

            std::cout << "线程 " << thread_id << " 成功获取调度锁，执行任务: "
                      << task_name << std::endl;

            // 模拟任务执行
            std::this_thread::sleep_for(
                std::chrono::milliseconds(75));

            std::cout << "线程 " << thread_id << " 完成任务: "
                      << task_name << std::endl;

            resource_mutex_.unlock();
            return true;
        } else {
            ++timeout_count_;
            std::cout << "线程 " << thread_id << " 在截止时间前未能获取调度锁，"
                      << "任务: " << task_name << " 被取消" << std::endl;
            return false;
        }
    }

    // 获取统计信息
    void getStatistics() const {
        std::cout << "\n资源访问统计:\n";
        std::cout << "成功访问次数: " << access_count_ << std::endl;
        std::cout << "超时次数: " << timeout_count_ << std::endl;
        std::cout << "超时比例: "
                  << static_cast<double>(timeout_count_) /
                     (access_count_ + timeout_count_) * 100
                  << "%" << std::endl;
    }
};

int main() {
    ResourceManager resource_manager(5);
    std::vector<std::thread> threads;

    // 创建多个工作线程
    for (int i = 0; i < 10; ++i) {
        threads.push_back(std::thread([&resource_manager, i]() {
            // 模拟不同类型的资源访问

            // 情况1: 短超时
            resource_manager.useResource(
                i, "操作-" + std::to_string(i),
                std::chrono::milliseconds(200));

            // 让线程错开执行
            std::this_thread::sleep_for(std::chrono::milliseconds(20));

            // 情况2: 使用unique_lock的超时锁定
            resource_manager.processData(
                i, "数据-" + std::to_string(i),
                std::chrono::milliseconds(150));

            // 情况3: 使用截止时间点
            auto deadline = std::chrono::system_clock::now() +
                            std::chrono::milliseconds(180);
            resource_manager.scheduleTask(
                i, "任务-" + std::to_string(i), deadline);
        }));
    }

    // 等待所有线程完成
    for (auto& thread : threads) {
        thread.join();
    }

    // 显示统计信息
    resource_manager.getStatistics();

    return 0;
}
```
