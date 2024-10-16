//
// Created by whx on 24-10-14.
//

#ifndef THREAD_HPP
#define THREAD_HPP

#include <chrono>
#include <thread>
#include <memory>
#include <iostream>
#include <functional>

// 前言：linux只有进程，基于进程实现的线程（轻量级的进程）
// 进程有自己的虚拟内存空间，而多个线程共用一片虚拟空间，虚拟空间介绍：https://subingwen.cn/linux/file-descriptor/#1-2-%E5%88%86%E5%8C%BA
// 线程：需要抢时间片，有就绪态和运行态
namespace Thread {
    using namespace std;

    /*******
     创建线程
     *******/

    // 创建线程的回调
    inline void* create_callback(void *arg) {
        cout << "son is be created" << endl;
        return nullptr;
    }

    // 创建线程，由于我们运行一段程序会产生一个进程，在进程开始执行后其会退化成主线程，
    // 因此我们创建的其实是子线程
    // 执行顺序：主线程一直在运行，子线程创建出来以后需要抢cpu时间片，抢不到就无法运行
    // 但是在抢到之前主线程若是退出，则会释放虚拟地址空间，导致子线程一并销毁，反之不同

    // 结论：默认情况下，主线程和虚拟地址空间的生命周期相同，而与子线程无关
    inline void create_thread() {
        // 线程ID: pthread_t: unsigned long int
        const pthread_t thread_1 = pthread_self();
        cout << thread_1 << endl;

        // 创建子线程：传出，属性，回调，实参(arg)
        pthread_t id;
        pthread_create(&id, nullptr, create_callback, nullptr);
    }

    // 创建的工作函数，通过休眠的方式延长生命周期
    inline void create_workspace() {
        const std::function<void()> main_callback = [&] { create_thread(); };
        main_callback();

        std::this_thread::sleep_for(std::chrono::seconds(1));
        cout << "----------------" <<endl;
    }

    /*******
     退出线程
     *******/

    // 线程的退出是不分主子的，即任意一个线程退出不影响其他线程
    inline void* exit_callback(void *arg) {
        cout << "son is exit" << endl;
        return nullptr;
    }

    // 退出主线程,退出数据会直接返回主线程
    inline void exit_workspace() {
        cout << pthread_self() << endl;

        // 创建一个子线程
        pthread_t id;
        pthread_create(&id, nullptr, exit_callback, nullptr);

        pthread_exit(nullptr);
    }


    /*******
     线程回收
     *******/

    // 线程回收原理：子线程的内核资源由其主线程进行回收，并可记录返回参数

    struct Recycle_Student {
        const int id = 10;
        const int age = 45;
        const std::string name = "LiLy";

        Recycle_Student() = default;
        virtual ~Recycle_Student() noexcept = default;
        Recycle_Student(const Recycle_Student &) = delete;
        Recycle_Student(const Recycle_Student &&) = delete;
        Recycle_Student& operator=(const Recycle_Student &) = delete;
        Recycle_Student& operator=(const Recycle_Student &&) = delete;
    };

    inline void *recycle_callback(void * arg) {
        cout << "this thread is be recycled" <<endl;
        std::shared_ptr<Recycle_Student> p = make_shared<Recycle_Student>();

        // 将这个要回收的学生对象返回主线程
        pthread_exit(p.get());
    }

    inline void recycle_workspace() {
        pthread_t id;
        pthread_create(&id, nullptr, recycle_callback, nullptr);

        // 承接子线程退出释放的数据
        void *ptr = nullptr;

        auto callback = [&]()->void {
            // 线程回收:pthread_t, 线程退出返回的数据
            // 整体可以通过如下流程使用第二个参数:通过pthread_exit的返回值释放子线程资源(返回一个资源地址)
            // 并通过void **__thread_return(join的参数二)访问这个地址

            // 上文需保证在同一片地址空间，比如同时使用主堆栈(这个实际是我们默认退出线程返回的位置)
            pthread_join(id, &ptr);
            const auto stu_tmp = static_cast<Recycle_Student *>(ptr);
            cout << stu_tmp->id << endl
                 << stu_tmp->age << endl
                 << stu_tmp->name << endl;
        };

        std::this_thread::sleep_for(std::chrono::seconds(1));
        callback();
        pthread_exit(nullptr);
    }

    /*******
     线程分离
     *******/

    inline void *detach_callback(void *arg) {
        cout << pthread_self() << endl;
        static int x = 1;
        while(x < 8) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            cout << x++ << endl;
        }
        return nullptr;
    }

    inline void detach_workspace() {
        pthread_t id;
        pthread_create(&id, nullptr ,detach_callback, nullptr);

        cout << "main thread" << endl;
        // pthread_detach(id);
        pthread_exit(nullptr);
    }

}

#endif //THREAD_HPP
