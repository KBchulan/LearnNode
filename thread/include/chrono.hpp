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

    // 原型构造：1.无参构造  2.指定时间间隔  3.拷贝构造（指定时钟和时间间隔）
    // 其实我们只需要知道他存储的是一个时间即可（和int* 存储的是一个整形地址一样）
    // 此外还有一个函数time_since_epoch，可以获取自纪元以来的时间间隔

    // 此时间点类和下文时钟类一起用，现在一个小例子
    inline void time_point_workspace() {
        const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

        const std::chrono::system_clock::time_point current_time = now;
        const std::time_t time = std::chrono::system_clock::to_time_t(current_time);

        std::cout << "Current time: " << std::ctime(&time);

        const std::chrono::system_clock::time_point futher = current_time + std::chrono::seconds(20);
        const std::time_t futher_time_t = std::chrono::system_clock::to_time_t(futher);

        std::cout << "Futher time: " << std::ctime(&futher_time_t);

        const auto duration = now.time_since_epoch();
        const auto mill = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        std::cout << mill << std::endl;
    }

    // 时钟clocks，我们的库中有三个时钟类：
    // system_clock: 系统时间，可以对时，因此可能不准确
    // steady_clock: 固定时钟，打开之后开始计时，且时间时钟增长（记录程序耗时）
    // high_resolution_clock: 其实就是第二类时钟的别名，不再介绍

    // 每一类时钟都有time_point,duration,rep,period四种类型，以及具体时间time_t
    // 和time_point之间的转换
    // 优势：我们不需要创建对象，直接调用类的静态方法即可

    // 系统时钟：rep为long long, period为100ns，周期绑定为long long的ns
    // 三个函数：now(返回、当前时间)，to_time_t(point->time_t),from_time_t(与第二个相反)
    inline void system_clock_workspace() {
        // 输出时间流程duration构造clock里的time_point，借助to_time_t->time_t，再化为c字符串输出

        // 新纪元1970.1.1时间
        constexpr std::chrono::system_clock::time_point epoch;

        // 新纪元过一天
        const std::chrono::duration<int, std::ratio<60 * 60 * 24>> day(1);
        const std::chrono::system_clock::time_point d_tomorrow(day);

        // 过10天
        const std::chrono::duration<int ,std::ratio<60 * 60 * 24>> day_02(10);
        const std::chrono::time_point<std::chrono::system_clock,
              std::chrono::duration<int ,std::ratio<60 * 60 * 24>>>
              d_ten(day_02);
        // 上面一个声明与const std::chrono::system_clock::time_point d_ten(day_02)等价

        // 当前时间
        std::chrono::time_point today = std::chrono::system_clock::now();

        const time_t aaa = std::chrono::system_clock::to_time_t(today);
        std::cout << "今天是： " << std::ctime(&aaa);

        const time_t bbb = std::chrono::system_clock::to_time_t(today + day);
        std::cout << "明天是： " << std::ctime(&bbb);

        const time_t ccc = std::chrono::system_clock::to_time_t(epoch);
        std::cout << "新纪元是： " << std::ctime(&ccc);

        const time_t ddd = std::chrono::system_clock::to_time_t(d_tomorrow);
        std::cout << "+1是： " << std::ctime(&ddd);

        const time_t eee = std::chrono::system_clock::to_time_t(d_ten);
        std::cout << "+10是： " << std::ctime(&eee);
    }

    // steady_clock:主要用于计算程序的耗时,在源码上相当于一个计时器
    // rep为long long,period为1ns,周期也是1ns
    inline void steady_clock_workspace() {
        const std::chrono::steady_clock::time_point begin
            = std::chrono::steady_clock::now();

        for(auto i = 0; i < 100; i++)
            std::cout << "*" << " ";
        std::cout << std::endl;

        const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        const auto delta = now - begin;
        std::cout << delta.count() << std::endl;
    }

    // high_resolution_clock如上文所说只是steady_clock的别名,无再次介绍的必要

    // 转换函数:
    // duration_cast可以对时钟周期类型或者周期次数类型进行修改
    // 注意:周期类型转换是高到底,如小时到分钟
    // 次数转换是低等到高等,如int->double
    inline void duration_cast_workspace() {
        auto func = []() {
            for(auto i = 0; i < 100; i++)
                std::cout << "*" << " ";
            std::cout << std::endl;
        };

        const auto t1 = std::chrono::steady_clock::now();
        func();
        const auto t2 = std::chrono::steady_clock::now();

        const auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

        const std::chrono::duration<double, std::ratio<1, 1000>> float_ms = t2 - t1;

        std::cout << "flopa :" << float_ms.count() << " "
                  << "int :" << int_ms.count() << std::endl;
    }

    // time_point_cast可以对时间点进行显式转换

    using Clock = std::chrono::steady_clock;
    using Ms = std::chrono::milliseconds;
    using Sec = std::chrono::seconds;
    template<typename Duration>
    using TimePoint = std::chrono::time_point<Clock, Duration>;

    inline void time_point_cast_workspace() {
        constexpr TimePoint<Ms> time_point_ms(Ms(6000));
        constexpr TimePoint<Sec> time_point_sec = std::chrono::time_point_cast<Sec>(time_point_ms);
        std::cout << time_point_sec.time_since_epoch().count() << std::endl;
    }


}

#endif //CHRONO_HPP
