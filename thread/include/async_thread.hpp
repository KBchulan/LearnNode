//
// Created by whx on 24-10-20.
//

#ifndef ASYNC_THREAD_HPP
#define ASYNC_THREAD_HPP

#include <future>
#include <thread>
#include <iostream>

// 同步是有一些弊端的，比如我们想要拿到子线程中间的数据，只能死等到线程结束，因此我们常常需要设计为异步

// 主线程中想要获取到子线程的任务返回函数，可以采用异步的方式，即我们c++11提供的std::funture类
namespace ASYNCTHREAD {
    // std::future
    // 是一个模板类，可以存储任意类型的数据，是为了在主线程中获取子线程返回的结果
    // 构造：无参构造，移动构造，不可拷贝构造

    // 成员函数：get()取出future保存的数据
    //         wait()阻塞当前线程，直到子线程执行完毕后将数据写入future对象后解锁
    //         wait_for()和wait_until()相信有了前面学习的基础，这个函数的功能应该十分简单易猜了把


    // std::promise
    // 是一个协助线程赋值的类，它能够将数据和future对象绑定起来，为获取线程函数中的某个值提供便利
    // 也是一个模板类，需要传递什么类型的数值。就传进去什么参数
    // 构造：和future一样

    // 成员函数：get_future()获取到promise管理的future对象
    //         set_value()存储要传出的值，并进入就绪态，可以让其他线程获取到这个数据
    //         set_value_at_thread_exit()在子线程结束并销毁资源后再就绪

    // 使用流程：create promise-->引用传给子线程的任务函数-->子线程中赋值-->主线程中取出promise里的future-->通过future拿到值
    inline void promise_workspace() {
        std::promise<int> pro;

        std::thread thr_1([&pro]() {
            pro.set_value_at_thread_exit(100);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout << "weak up..." << std::endl;
        });

        std::future<int> fu = pro.get_future();
        std::cout << fu.get() << std::endl;
        thr_1.join();
    }

    // std::packaged_task，包装了一个可调用对象包装器类对象，可以将函数与future绑定到一起
    // 与promise的区别：promise保存的是一个共享状态的数值，而packager_task保存的则是一个函数

    // 构造：无参，有参（可调用对象），移动构造
    // 常用公共函数：get_future()获取future对象
    inline void packaged_task_workspace() {
        std::packaged_task<int(int)> task([](int x) {
            return x += 200;
        });

        // ref可以确保对象的引用传递
        std::thread thr_01(std::ref(task), 100);

        std::future<int> fur = task.get_future();
        std::cout << fur.get() << std::endl;

        thr_01.join();
    }

    // std::async这个比较高级，可以直接启动一个子线程并在子线程中执行任务函数，执行完后
    // 的结果也是存储到一个future里面
    // 构造：1.传递可调用对象 2.传递执行策略   均返回一个future对象
    // 执行策略：std::launch::async调用async是创建新的线程并执行任务函数
    // std::launch::defered 调用future的get或者wait才执行任务

    inline void async_workspace_right_now() {
        std::cout << "main thread is:" << std::this_thread::get_id() << std::endl;

        std::future<int> func = std::async([] (int x){
            std::cout << "son thread is:" << std::this_thread::get_id() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(8000));
            return x += 100;
        }, 100);

        std::future_status status;

        do {
            status = func.wait_for(std::chrono::seconds(1));
            if(status == std::future_status::deferred) {
                std::cout << "线程还没有执行..." << std::endl;
                func.wait();
            }
            else if(status == std::future_status::ready) {
                std::cout << "return value is:" << func.get() << std::endl;
            }
            else {
                std::cout << "wait......." << std::endl;
            }
        }
        while(status != std::future_status::ready);

    }

}

#endif //ASYNC_THREAD_HPP
