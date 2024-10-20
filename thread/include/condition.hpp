//
// Created by whx on 24-10-19.
// 此文件我们记录c++11的条件变量和原子变量
//

#ifndef CONDITION_HPP
#define CONDITION_HPP

#include <list>
#include <mutex>
#include <memory>
#include <iostream>
#include <condition_variable>

// c++11提供了两种条件变量：condition_variable（需要和std::unique_lock<std::mutex>结合使用）
// condition_variable_any（可以和任意一个带有lock和unlock语义的互斥锁结合使用->可以去看同目录下的
// multhread.hpp里面的笔记）

// 应用场景还是生产者消费者模型，此处不再介绍，c风格的里面已经记录过

// condition_variable:包括阻塞函数和线程通知函数
// wait的参数选择有两种，只传入一个互斥锁（unique_lock），或者传入锁和一个返回值为布尔类型的函数（
// 次函数可以是有名函数，也可以是匿名函数），函数返回false则表示线程被阻塞；
// wait_for比上面那个多了个duration对象的阻塞时长,当被阻塞的线程超过等待时长时，会被唤醒而继续向下执行
// 唤醒函数：notify_one唤醒一个被当前条件变量阻塞的线程，notify_all唤醒所有的

namespace CONDITION {
    class SyncQueue {
    public:
        explicit SyncQueue(const int& maxsize) : m_maxSize(maxsize) {}
        ~SyncQueue() = default;

        void put(const int &x) {
            std::unique_lock<std::mutex> locker(m_mutex);
            while(m_queue.size() == m_maxSize) {
                std::cout << "任务队列已满...." << std::endl;
                m_notFull.wait(locker);
            }
            std::cout << "放入数据：" << x << std::endl;
            m_queue.push_back(x);
            m_notEmpty.notify_all();
        }

        int pop() {
            std::unique_lock<std::mutex> locker(m_mutex);
            while(m_queue.empty()) {
                std::cout << "任务队列已kong...." << std::endl;
                m_notEmpty.wait(locker);
            }
            const int x = m_queue.front();
            std::cout << "取出数据:" << x << std::endl;
            m_queue.pop_front();
            m_notFull.notify_all();
            return x;
        }


        bool empty() {
            std::lock_guard<std::mutex> locker(m_mutex);
            return m_queue.empty();
        }

        bool full() {
            std::lock_guard<std::mutex> locker(m_mutex);
            return (m_queue.size() == m_maxSize);
        }

        int size() {
            std::lock_guard<std::mutex> locker(m_mutex);
            return static_cast<int>(m_queue.size());
        }

    private:
        std::list<int> m_queue;
        std::mutex m_mutex;
        std::condition_variable m_notEmpty;
        std::condition_variable m_notFull;
        int m_maxSize;
    };

    inline void condition_workspace() {
        const auto sync_queue = std::make_shared<SyncQueue>(50);

        auto produce = [capture0 = sync_queue.get()]
                <typename T0>(T0 && PH1){
                capture0->put(std::forward<T0>(PH1));
            };

        auto consume = [capture0 = sync_queue.get()]
            { return capture0->pop(); };

        std::thread producer[3], consumer[3];

        for(auto i = 0; i < 3; i++) {
            producer[i] = std::thread(produce, i * 2 + 100);
            consumer[i] = std::thread(consume);
        }

        for(auto i = 0 ;i < 3; i++) {
            producer[i].join();
            consumer[i].join();
        }
    }

    // condition_variable_any:与上文那个是一样的，不过函数的实参可以是多种锁,故此处不再介绍

/*
    总结：以上介绍的两种条件变量各自有各自的特点，condition_variable 配合
    unique_lock使用更灵活一些，可以在在任何时候自由地释放互斥锁，
    而condition_variable_any 如果和lock_guard 一起使用必须要等到其生命周期结束才能将互斥锁释放。
    但是，condition_variable_any 可以和多种互斥锁配合使用，应用场景也更广，
    而 condition_variable 只能和独占的非递归互斥锁（mutex）配合使用，有一定的局限性。
*/
}

namespace ATOMIC {
    // c++11引入的原子类型管理的变量称为原子变量（不包括浮点类型和符合类型）
    // 原子之的是不可被多个cpu交换的机器指令（即一块CPU进行原子操作时，其他CPU内核会暂停对此内核的操作），由于其是基于指令提供的支持，性能会好很多，而且
    // 几乎所有语言都支持原子类型，同时原子是无锁类型，但是由于内部使用了CAS循环，当发生大量
    // 冲突时，还是需要等待的，但是比锁好

    // c++11内置了整形的原子变量，使用还是很简单的，在多线程中使用比较好，因为
    // 原子操作指的是不会被线程调度机制打断的操作，这种操作一旦开始，就一直运行到结束，中间不会有任何的上下文切换
    // 也就不需要互斥锁来实现这个了

    // 构造：atimic<type>，要指定类型;  ----不允许拷贝构造
    // 公共函数：store：传入desire（值），按照order的方式影响内存
    // load：原子的加载并返回原子变量的值，按照order的方式影响内存，其实直接访问原子对象就行了
    // 运算符重载的特化函数，点开源码看看就行，没啥说的必要，感觉有什么操作用一下就知道了
    // 内存顺序约束：上文的order：源码里有一个memory_order，下面是大体介绍

    /**
    memory_order_relaxed， 这是最宽松的规则，它对编译器和CPU不做任何限制，可以乱序
    memory_order_release 释放，设定内存屏障(Memory barrier)，保证它之前的操作永远在它之前，但是它后面的操作可能被重排到它前面
    memory_order_acquire 获取, 设定内存屏障，保证在它之后的访问永远在它之后，但是它之前的操作却有可能被重排到它后面，往往和Release在不同线程中联合使用
    memory_order_consume：改进版的memory_order_acquire ，开销更小
    memory_order_acq_rel，它是Acquire 和 Release 的结合，同时拥有它们俩提供的保证。比如你要对一个 atomic 自增 1，同时希望该操作之前和之后的读取或写入操作不会被重新排序
    memory_order_seq_cst 顺序一致性， memory_order_seq_cst 就像是memory_order_acq_rel的加强版，它不管原子操作是属于读取还是写入的操作，只要某个线程有用到memory_order_seq_cst 的原子操作，线程中该memory_order_seq_cst 操作前的数据操作绝对不会被重新排在该memory_order_seq_cst 操作之后，且该memory_order_seq_cst 操作后的数据操作也绝对不会被重新排在memory_order_seq_cst 操作前。
    **/

    // 20之后可以借助原子来阻塞，如有需要请看作者20笔记
    inline void atomic_workspace() {
        const std::atomic_int a = 5;
        const std::atomic_int b = 6;
        const std::atomic_int c = a + b;
        std::cout << c << std::endl;
    }

    // 下面有一个计数的例子
    // 互斥锁版本
    inline void count_with_mutex() {
        static int64_t num = 0;
        static std::mutex my_mutex;

        auto increase = []() {
            for(auto i = 0; i < 10; i++) {
                std::lock_guard<std::mutex> locker(my_mutex);
                std::cout << "the num increase to:" << ++num << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(800));
            }
        };

        auto decrease = []() {
            for(auto i = 0; i < 10; i++) {
                std::lock_guard<std::mutex> locker(my_mutex);
                std::cout << "the num decrease to:" << --num << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(800));
            }
        };

        std::thread thr_01(increase);
        std::thread thr_02(decrease);

        thr_01.join();
        thr_02.join();
    }

    // 原子变量版本
    inline void count_with_atomic() {
        std::atomic_int num = 0;

        auto increase = [&num]() {
            for(auto i = 0; i < 10; i++) {
                std::cout << "the num increase to:" << ++num << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(800));
            }
        };

        auto decrease = [&num]() {
            for(auto i = 0; i < 10; i++) {
                std::cout << "the num decrease to:" << --num << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(800));
            }
        };

        std::thread thr_01(increase);
        std::thread thr_02(decrease);

        thr_01.join();
        thr_02.join();
    }

}


#endif //CONDITION_HPP
