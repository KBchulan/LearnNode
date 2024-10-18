//
// Created by whx on 24-10-14.
// 该库可以很方便的处理时间
//

#ifndef CHRONO_HPP
#define CHRONO_HPP

#include <chrono>
#include <iostream>

namespace CHRONO {
    // duration：是一个模板，表示一段时间间隔，<Reg,Ratio（单位为秒）>: <数值（周期）类型，（分子，分母 = 1）>

    // 表示时间周期，但是我们没必要通过自己手动计算需要多少秒并进行单位转换，有很多库函数，如：
    // std::chrono::nanoseconds(100);      // 100纳秒
    // std::chrono::microseconds(100);     // 100微秒
    // std::chrono::milliseconds(100);     // 100毫秒
    // std::chrono::seconds(100);          // 100秒
    // std::chrono::minutes(100);          // 100分钟
    // std::chrono::hours(100);            // 100小时
    // 库函数可以涵盖上下292年的范围

    // 下面说一下duration的构造呢：1.拷贝构造 2.指定周期类型 3.指定时钟周期类型和具体长度
    // (周期类型， 每个周期时间， 周期数目)，至少有数目一个属性
    // 重载运算：赋值，+，++，-，--，+=，-=（可以看源码，此处只是提一下）
    // 还有一个count函数可以得到时钟周期数目

    inline void duration_workspace() {
        const std::chrono::duration<double> dura_01(6.6);                     // 6秒
        const std::chrono::duration<double, std::ratio<6>> dura_02(1);        // 6秒
        const std::chrono::duration<double, std::ratio<6, 2>> dura_03(5);     // 每个周期3秒，共5个周期

        const std::chrono::duration<double, std::ratio<1, 30>> dura_04(1.5);  // 数据类型和周期数类型要保持一致

        const std::chrono::seconds s1(60), s2(20);
        const std::chrono::seconds s3 = s1 - s2;
        std::cout << dura_01.count() << " "
                  << dura_02.count() << " "
                  << dura_03.count() << " "
                  << dura_04.count() << " "
                  << s3.count()      << " "
                  << std::endl;
    }

    // 时间点：time point，也是模板，存储一个duration，自Clock的纪元开始的时间间隔的
    // 值，通过类可以返回一个时间点，Clock是计量的时钟，duration则是时间间隔

    // 原型构造：1.无参构造  2.构造
    // 其实我们只需要知道他存储的是一个时间即可（int一样）

    // 此时间点类和下文时钟类一起用，现在一个小例子
    inline void time_point_workspace() {
        const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

        const std::chrono::system_clock::time_point current_time = now;
        const std::time_t time = std::chrono::system_clock::to_time_t(current_time);

        std::cout << "Current time: " << std::ctime(&time);

        const std::chrono::system_clock::time_point futher = current_time + std::chrono::seconds(20);
        const std::time_t futher_time_t = std::chrono::system_clock::to_time_t(futher);

        std::cout << "Futher time: " << std::ctime(&futher_time_t);
    }


}

#endif //CHRONO_HPP
