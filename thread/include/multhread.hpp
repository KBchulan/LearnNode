//
// Created by whx on 24-10-16.
// 本文件为线程同步内容
//

// 线程同步初认知，并不是让多个线程同时去干某件事
// 而是让多个线程按照先后顺序去干一件事，整体而言是将线程排队，这样确实会降低访问速度，
// 但是能够提高安全性，是值得的

// 总结：线程同步是让线程线性处理，而非并行处理

#ifndef MULTHREAD_HPP
#define MULTHREAD_HPP

#include <list>
#include <mutex>
#include <chrono>
#include <thread>
#include <iostream>
#include <semaphore>
// #include <functional>

namespace MULTHREAD {
    using namespace std;

    // 初认知，多线程计数

    // 先说现象：A，B两线程竞争计数，出现重复甚至回退计数
    // 原理：多个线程是分时复用CPU的时间片，而且有sleep_for使时间片被迫挂起，从而
    // 更容易出现数据混乱的情况，抢到了时间片的线程就会开始计数
    // 这里先说一下数据的来源：从物理内存加载来的，物理内存经三层缓存到达CPU寄存器，也可以通过
    // I/O读写到磁盘中，我们关注左侧流程，A线程开始，物理内存的数据（全局变量在.data段）到达CPU寄存器进行
    // 处理，最后再返回物理内存，在时间片结束时，CPU被挂起，会导致缓存位置的数据没有返回到物理
    // 内存中，因此B读到的数据会是上一次已经被写入memory的数据，从而导致重复计数

    // 线程同步就是确保我们的数据被同步到memory后再执行其他的线程
    // 同步方式： 互斥锁， 读写锁， 条件变量， 信号量
    // 可以这么理解：有一段任务代码，只有拥有钥匙的人才能打开锁去执行，但是钥匙只有一把

    inline int64_t num = 0;

    inline void *count_01_callback(void *arg) {
        while (num <= 10) {
            cout << "Thread A : " << pthread_self()
                 << " " << num++ << endl;
            this_thread::sleep_for(chrono::seconds(1));
        }
        return nullptr;
    }

    inline void *count_02_callback(void *arg) {
        while (num <= 10) {
            cout << "Thread B : " << pthread_self()
                 << " " << num++ << endl;
            this_thread::sleep_for(chrono::seconds(1));
        }
        return nullptr;
    }

    inline void count_workspace() {
        pthread_t thr_01, thr_02;

        pthread_create(&thr_01, nullptr , count_01_callback, nullptr);
        pthread_create(&thr_02, nullptr , count_02_callback, nullptr);

        pthread_join(thr_01, nullptr);
        pthread_join(thr_02, nullptr);
    }

    // 互斥锁：锁的拥有权只能是一个线程

    // 我们先介绍一下__restrict的用法：限制指针，通过该指针访问的内存区域不能被其他指针
    // 修改，这样在大型数组和复杂数据的情况下是十分优秀的，同时也能明确表达意图，但是
    // 建议是裸指针，因为他这个限制的方式是和shared_ptr相悖的，别忘了释放内存哦
    inline void restrict_learn() {
        int x = 5;
        int *__restrict a = &x;
        // 下面这一句会有一些问题,因为指向的内存重复了
        int *__restrict b = &x;
        (*b)++;
        cout << *b << endl;

        delete a;
        delete b;
    }

    // 确认临界区：对共享资源的使用部分(临界资源)
    // pthread_mutex_t 创建互斥锁
    // pthread_mutex_init 初始化互斥锁
    // pthread_mutex_destroy 释放互斥锁资源
    // pthread_mutex_lock 加锁，先判断锁的状态，若是未加锁，则对线程进行加锁，反之则阻塞线程
    // pthread_mutex_trylock 与上一个不同，不会阻塞，会返回错误
    // pthread_mutex_unlock 被加锁的线程才可以解锁

    // 借用上面的num,我们会发现一个很有趣的现象，我们的B最后输出了101,可以让我们知道，线程是会阻塞在lock函数
    // 那一行上面的
    inline pthread_mutex_t mutex;

    inline void *count_01_with_mutex_callback(void *arg) {
        while(num <= 100) {
            pthread_mutex_lock(&mutex);
            cout << "Thread A : " << pthread_self()
                 << " " << num++ << endl;
            // this_thread::sleep_for(chrono::seconds(1));
            pthread_mutex_unlock(&mutex);
        }
        return nullptr;
    }

    inline void *count_02_with_mutex_callback(void *arg) {
        while(num <= 100) {
            pthread_mutex_lock(&mutex);
            cout << "Thread B : " << pthread_self()
                 << " " << num++ << endl;
            // this_thread::sleep_for(chrono::seconds(1));
            pthread_mutex_unlock(&mutex);
        }
        return nullptr;
    }

    inline void mutex_workspace() {
        pthread_t thr_01, thr_02;

        pthread_mutex_init(&mutex, nullptr);

        pthread_create(&thr_01, nullptr, count_01_with_mutex_callback, nullptr);
        pthread_create(&thr_02, nullptr, count_02_with_mutex_callback, nullptr);

        pthread_join(thr_01, nullptr);
        pthread_join(thr_02, nullptr);

        pthread_mutex_destroy(&mutex);
    }

    // 线程死锁：由于我们同步采用的方式不当，导致所有线程都被阻塞了，即程序挂起

    // 原因：1.加锁之后忘记解锁
    inline void reason_01() {
        while(num <= 10) {
            pthread_mutex_lock(&mutex);
            num++;
            // 此处未解锁就会导致死锁，程序必须在执行完临界区后执行解锁操作
        }
    }

    // 原因2：重复加锁
    inline void reason_02() {
        // 简单情况
        while(num <= 10) {
            pthread_mutex_lock(&mutex);
            num++;
            // 在执行下面一行时即被阻塞
            pthread_mutex_lock(&mutex);

            pthread_mutex_unlock(&mutex);
        }

        // 稍复杂的情况
        auto callback = [](int64_t &i) {
            pthread_mutex_lock(&mutex);
            i++;
            pthread_mutex_unlock(&mutex);
        };

        while(num <= 10) {
            pthread_mutex_lock(&mutex);
            callback(num);
            pthread_mutex_unlock(&mutex);
        }
    }

    // 原因3：具有许多共享资源，因此需要多个锁，随意加锁导致死锁
    // 我们的锁的个数等于我们共享资源的个数，说白了就是我的线程A的解锁需要访问一个共享资源
    // 但是访问的在这个共享资源已经被其他线程加锁

    // 读写锁：互斥锁的pro版本，可以指定读操作还是写操作，因为我们的读操作完全可以并行，只有
    // 写操作才需要串行，锁包括打开和锁两个状态，其中又分读还是写，最后还能记录具体哪个线程上锁
    // 写操作等级优于读操作,且一把锁只能被读锁或者被写锁
    // 场景：大量的读操作用读写锁，反之两者都行

    // 函数与互斥锁比较一致，此处不再介绍（看了介绍应该都能猜到）
    inline pthread_rwlock_t rwlock;     // .bss段

    inline void *write_callback(void *arg) {
        while(num <= 30) {
            pthread_rwlock_wrlock(&rwlock);
            cout << "write: " << pthread_self() << " " << num++ << endl;
            pthread_rwlock_unlock(&rwlock);
            usleep(random() % 500);
        }
        return nullptr;
    }

    inline void *read_callback(void *arg) {
        while(num <= 30) {
            pthread_rwlock_rdlock(&rwlock);
            cout << "read: " << pthread_self() << " " << num++ << endl;
            pthread_rwlock_unlock(&rwlock);
            usleep(random() % 500);
        }
        return nullptr;
    }

    inline void rwlock_workspace() {
        pthread_rwlock_init(&rwlock, nullptr);

        pthread_t write_thr[3],
                  read_thr[5];

        for (auto& write : write_thr)
            pthread_create(&write, nullptr, write_callback, nullptr);

        for (auto& read : read_thr)
            pthread_create(&read, nullptr, read_callback, nullptr);

        for (const auto& write : write_thr)
                pthread_join(write, nullptr);

        for (const auto& read : read_thr)
                pthread_join(read, nullptr);

        pthread_rwlock_destroy(&rwlock);
    }

    // 条件变量函数
    // 严格来说，条件变量并不是实现线程同步，而是进行线程的阻塞
    // 与互斥锁不同之处：假设有100个线程，互斥锁会使其中一个线程加锁成功而阻塞其他线程，从而确保
    // 线程同步，但是条件变量会使满足条件的所有线程都进入临界区，同时进行读写操作，这显然会有一些问题
    // 可以与锁共同使用（互斥锁）处理生产者和消费者模型（应用场景）
    // （多线程）生产者->任务队列<-消费者（多线程）

    // condition储存阻塞的线程信息
    // pthread_cond_init 初始化
    // pthread_cond_destroy 释放资源
    // pthread_cond_wait 线程阻塞函数，调用此函数会阻塞线程
    // pthread_cond_timedwait 上面那个阻塞，只要不唤醒就会一直阻塞，这个有时间限制
    // pthread_cond_signal 唤醒至少一个阻塞线程
    // pthread_cond_broadcast 唤醒全部阻塞函数

    // 以下为一个生产者（5）-消费者（5）模型
    inline pthread_cond_t cond_from_create;
    inline pthread_cond_t cond_from_cost;
    // 上文有一个mutex的互斥锁

    inline void condition_callback() {
        // 初始化
        pthread_cond_init(&cond_from_cost, nullptr);
        pthread_cond_init(&cond_from_create, nullptr);
        pthread_mutex_init(&mutex, nullptr);

        static list<int> my_list;

        pthread_t createres[5],
                  costeres[5];

        auto create = [](void *arg)->void* {
            while(my_list.size() <= 50) {
                pthread_mutex_lock(&mutex);

                while(my_list.size() >= 20) {
                    // 若是被阻塞，则会释放这个互斥锁
                    pthread_cond_wait(&cond_from_create, &mutex);
                }

                my_list.emplace_back(forward<int>(static_cast<int>(random() % 50)));
                cout << "creater :" << " " << pthread_self() << " "
                     << my_list.back() << endl;

                pthread_mutex_unlock(&mutex);
                pthread_cond_broadcast(&cond_from_cost);
                this_thread::sleep_for(chrono::seconds(forward<int>(static_cast<int>(random() % 3))));

            }
            return nullptr;
        };

        auto cost = [](void *arg)->void* {
            while(my_list.size() <= 50) {
                pthread_mutex_lock(&mutex);

                while(my_list.empty()) {
                    // 若是被阻塞，则会释放这个互斥锁
                    pthread_cond_wait(&cond_from_cost, &mutex);
                }

                const int begin = my_list.front();
                my_list.pop_front();
                cout << "coster :" << " " << pthread_self() << " "
                     << begin << endl;

                pthread_mutex_unlock(&mutex);
                pthread_cond_broadcast(&cond_from_create);
                this_thread::sleep_for(chrono::seconds(forward<int>(static_cast<int>(random() % 3))));
            }
            return nullptr;
        };

        for (auto &creater : createres)
            pthread_create(&creater, nullptr, create, nullptr);

        for (auto &coster : costeres)
            pthread_create(&coster, nullptr, cost,nullptr);

        for (const auto& creater : createres)
            pthread_join(creater, nullptr);

        for (const auto& coster : costeres)
            pthread_join(coster, nullptr);

        // 释放
        pthread_cond_destroy(&cond_from_cost);
        pthread_cond_destroy(&cond_from_create);
        pthread_mutex_destroy(&mutex);
    }

    // 信号量：与条件变量应用场景差不多，但是更轻量化

    // sem_t 创建信号量
    // sem_init 地址，（0为线程同步，非0为进程同步），资源数目
    // sem_destroy 销毁信号量
    // sem_wait 资源数量为0则阻塞，其余情况资源数-1
    // sem_trywait 资源数为0还不阻塞
    // sem_post 资源数+1
    // sem_getvalue

    // 创建两个信号量
    inline sem_t sem_from_create;
    inline sem_t sem_from_cost;

    inline void sem_workspace() {
        sem_init(&sem_from_cost, 0 ,0);
        sem_init(&sem_from_create, 0, 5);

        pthread_mutex_init(&mutex, nullptr);

        pthread_t createres[5],
                  costeres[5];

        static list<int> my_list;

        auto create = [](void *arg)->void* {
            while(my_list.size() <= 30) {
                sem_wait(&sem_from_create);
                pthread_mutex_lock(&mutex);
                my_list.emplace_back(forward<int>(static_cast<int>(random() % 5000)));
                cout << "creater :" << " " << pthread_self() << " "
                     << my_list.back() << endl;
                pthread_mutex_unlock(&mutex);

                sem_post(&sem_from_cost);
                this_thread::sleep_for(chrono::seconds(forward<int>(static_cast<int>(random() % 3))));
            }
            return nullptr;
        };

        auto cost = [](void *arg)->void* {
            while(my_list.size() <= 30) {
                sem_wait(&sem_from_cost);

                pthread_mutex_lock(&mutex);
                const int begin = my_list.front();
                my_list.pop_front();
                cout << "coster :" << " " << pthread_self() << " "
                     << begin << endl;
                pthread_mutex_unlock(&mutex);

                sem_post(&sem_from_create);
                this_thread::sleep_for(chrono::seconds(forward<int>(static_cast<int>(random() % 3))));
            }
            return nullptr;
        };

        // 创建和回收
        for (auto &creater : createres)
            pthread_create(&creater, nullptr, create, nullptr);

        for (auto &coster : costeres)
            pthread_create(&coster, nullptr, cost,nullptr);

        for (const auto& creater : createres)
            pthread_join(creater, nullptr);

        for (const auto& coster : costeres)
            pthread_join(coster, nullptr);

        pthread_mutex_destroy(&mutex);
        sem_destroy(&sem_from_cost);
        sem_destroy(&sem_from_create);

    }

}

#endif //MULTHREAD_HPP
