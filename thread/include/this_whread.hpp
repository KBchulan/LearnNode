//
// Created by whx on 24-10-19.
//

#ifndef THIS_WHREAD_HPP
#define THIS_WHREAD_HPP

// c++11不止引入了线程类，还有一个关于线程的命名空间std::this_thread，里面有四个公共的成员函数
// 我们可以对当前线程进行操作

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

}


#endif //THIS_WHREAD_HPP
