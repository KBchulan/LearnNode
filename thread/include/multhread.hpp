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

// #include <mutex>
#include <chrono>
#include <thread>
#include <iostream>
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
    // 访问，这样在大型数组和复杂数据的情况下是十分优秀的，同时也能明确表达意图，但是
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
    inline void mutex_workspace() {
        pthread_mutex_t mutex_01;
        // pthread_mutex
    }






}

#endif //MULTHREAD_HPP
