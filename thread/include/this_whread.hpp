//
// Created by whx on 24-10-19.
//

#ifndef THIS_WHREAD_HPP
#define THIS_WHREAD_HPP

// c++11不止引入了线程类，还有一个关于线程的命名空间std::this_thread，里面有四个公共的成员函数
// 我们可以对当前线程进行操作

#include <chrono>
#include <thread>
#include <iostream>

namespace THISTHREAD {
    using namespace std;

    // get_id:获取当前线程对应ID
    inline void get_id_workspace() {
        auto func = []() {
            cout << "son thread_id is :" << this_thread::get_id() << endl;
        };

        cout << "father thread is: " << this_thread::get_id() << endl;
        std::thread son(func);
        son.join();
    }

    // sleep_for
    // 线程被创建后有创建态，就绪态，运行态，阻塞（挂起）态->比如I/O或者一些锁的情况下，终止态，
    // 调用此函数会使线程从运行态变为阻塞态,参数为一个duration对象
    inline void sleep_for_workspace() {
        std::chrono::duration<int> dura(1);
        auto func = [&]() {
            for (int i = 0; i < 10 ;i++) {
                std::cout << "son thread is:" << std::this_thread::get_id() << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(dura));
            }};

        std::thread thr(func);
        thr.join();
    }

    // sleep_until与上面那个休眠的一样，不过此函数接受的是一个time_point对象
    inline void sleep_until_workspace() {
        const std::chrono::seconds sec(3);

        auto func = [&]() {
            for(int i = 0; i < 10; i++) {
                const auto now = std::chrono::steady_clock::now();
                std::cout << std::this_thread::get_id() << std::endl;
                std::this_thread::sleep_until(now + sec);
            }
        };

        std::thread thr(func);
        thr.join();
    }

    // yield:这是一个十分绅士的函数，调用此函数后，运行态的线程会让出自己抢到的cpu时间片
    // 从运行态变成就绪态，但是只会发挥一次作用
    inline void yield_workspace() {
        const std::chrono::duration<int> dura(1);

        auto func = [&]() {
            for(int i = 0; i < 10; i++) {
                std::cout << "son thread is :" << std::this_thread::get_id() << std::endl;
                std::this_thread::yield();
                std::this_thread::sleep_for(dura);
            }
        };

        std::thread thr_01(func);
        std::thread thr_02(func);
        thr_01.join();
        thr_02.join();
    }

    // call_once：在某些特定情况下，某些函数在多线程环境下只能被调用一次（如某个对象
    // 只需要初始化一次），此函数就是为了实现这个功能，在调用call_once时，通常需要传入
    // 一个once_flag参数,一个回调函数，一系列参数（不一定有）

    inline void call_once_workspace() {
        auto do_once = [](const int& a, const int& b) {
            std::cout << a << " " << b << std::endl;
        };

        auto callback = [do_once]<typename T0, typename T1>(T0 &&PH1, T1 &&PH2)
        { do_once(std::forward<T0>(PH1), std::forward<T1>(PH2)); };

        std::once_flag g_flag;
        std::once_flag gf_flag;
        auto do_something = [&](const int& a, const int& b) {
            static int num = 1;
            std::call_once(g_flag, do_once, a, b);
            std::call_once(gf_flag, callback, 6, 9);
            cout << "this is something:" << num++ << endl;
        };

        std::thread thr_01(do_something, 2, 3);
        std::thread thr_02(do_something, 4, 5);
        std::thread thr_03(do_something, 6, 7);

        thr_01.join();
        thr_02.join();
        thr_03.join();
    }

}


#endif //THIS_WHREAD_HPP
