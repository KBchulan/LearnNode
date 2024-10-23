//
// Created by whx on 24-10-23.
//

#ifndef C_20_HPP
#define C_20_HPP

#include <tuple>
#include <vector>
#include <ranges>
#include <variant>
#include <numeric>
#include <optional>
#include <iostream>
#include <algorithm>
#include <functional>

namespace CXX20 {
    using namespace std;

    // concept
    // 新特性最有用的一集（doge）
    // 20推出的检查模板参数类型,在编译期进行检查，我们一会会引入模板编程的很重要的知识点，而不是仅仅写concept

    // 请先看下面这个函数
    template<typename T>
    decltype(auto) GetLength(const T& t) {
        auto it = t.begin();
        auto end = t.end();
        size_t result = 0;
        for(; it != end; ++it, result++) {}
        return result;
    }
    // 我们会发现尽管我们已经显式的告诉编译器这么一个迭代器类型了，但是我们期望的是一个既有
    // iterator和成员函数begin和end的一个数据类型，如果我们传入的东西没有这么几个成员呢
    inline decltype(auto) func_01() {
        const std::vector aaa = {1, 2, 3};
        cout << GetLength(aaa) << endl;
        cout << GetLength("Hello"s) << endl;
        // 上面都还没有问题，但是我们指定一个不同的数据类型,下面这一行会报一大堆错误
        // cout << GetLength(123) << endl;
    }
    // 那么我们就会有这么一个想法，有没有什么东西能约束传入模板的参数类型，如果不满足我们期望
    // 那我们就压根不进行模板实例化，从而避免上述报错，ok，我接下来介绍两种方案，建议都看一下

    // SFINAE原则
    // 概括：解析函数模板重载时，如果无法替换模板的参数，则寻找下一个重载，而不是抛出编译错误
    // 其实说白了就是重载的函数模板匹配（跟普通的函数重载没啥大区别）

    struct Test_02 {
        typedef int foo;
    };

    template <typename T>
    // 要求T有内嵌类型foo
    void f(typename T::foo){ }

    template <typename T>
    void f(T) {}

    inline decltype(auto) func_02() {
        // 找名字->看参数得到解析集，若为空则会报错
        f<int>(10);
        f<Test_02>(20);
    }

    // 那么我们就可以利用SFINAE原则做一些编译期的判断，如类内定义内嵌类型了吗，是否定义了
    // 成员函数的名字
    template <typename T>
    struct has_reverse {
        // 定义字节数不同的内嵌类型
        struct good {char dummy; };
        struct bad {char dummy[2]; };

        template <typename U, float (U::*)()>
        struct SFINAE { };

        template <typename U>
        // 接受一个类型为U，且有一个返回float的成员函数指针
        static good test(SFINAE<U, &U::reverse>*);

        template <typename>
        static bad test(...);

        static const bool value = sizeof(test<T>(nullptr)) == sizeof(good);
    };

    class TestReverse_03 {
    public:
        float reverse();

    private:
        std::optional<int> a = std::make_optional(5);
    };

    class Bar_03 {
    public:
        std::variant<int> type = 3;
    };

    inline decltype(auto) func_03() {
        cout << has_reverse<TestReverse_03>::value << endl;
        cout << has_reverse<Bar_03>::value << endl;
    }

    // 其实还有enable_if，但是他并不是重点了，由此，我们就说明白了如何通过SFINAE原则
    // 去标定具有特定成员的类型，如不满足，则不进行实例化
    // 不过，是不是还感觉有些麻烦呢,那就请出我们的新主角concept，下面我们将开始说明
    // concept如何在编译期被计算并对模板进行约束

    template <typename T, typename U>
    // 约束T是U的派生类
    concept Derived = std::is_base_of_v<U, T>;

    template <typename T>
    // 约束T应当具有begin和end两个成员函数
    concept refer = requires(T t)
    {
        t.begin();
        t.end();
        t.size() > 3;
    };

    template <typename T>
    concept refer_02 = requires(T t)
    {
        std::get<int>(t) > 56;
        std::visit([](auto& arg) {
            arg > 100;
        }, t);
    };

    // ok，至此我们已经说清楚了concept这个东西是干嘛的：说白了就是约束模板，下面我们终于
    // 可以开始说用法了
    // 将requires写在函数后面
    template <typename T>
    size_t len_01(T t) requires refer<T> {
        return t.size();
    }

    // 将requires写在template下面
    template <typename T>
    requires refer_02<T>
    decltype(auto) print_info(T t) {
        std::visit([](auto& arg) {
            cout << arg << endl;
        }, t);
    }

    // 直接将concept替换typename/class
    // template <refer T>

    inline decltype(auto) func_04() {
        cout << len_01<std::string>("hello"s) << endl;
        const std::variant<int> a = 10;
        print_info<std::variant<int>>(a);
    }

    template <typename T>
    concept aaaa = requires(T t)
    {
        t.name != std::nullopt;
    };

    template <aaaa T>
    decltype(auto) print_name(const T& t) {
        cout << t.name.value() << endl;
    }

    struct Teacher {
        std::optional<std::string> name{};
    };

    inline decltype(auto) func_05() {
        const Teacher teacher = {"xiaoming"};
        cout << teacher.name.value() << endl;

        print_name(teacher);
    }

    // 此外，我们的concept还可以用&&或者||修饰
    // requires就是约束我们的模板或者模板参数


    // range library
    // ranges:可以简化我们的代码，同时还可以防止begin和end两个迭代器不配套使用，
    // 下面以其具体功能说一下

    // 简化操作
    inline decltype(auto) func_06() {
        std::vector<int> nums = {1, 2, 33, 4, 5};
        // sort
        std::sort(nums.begin(), nums.end());

        for(const auto& num : nums)
            cout << num << " ";
        cout << endl;

        // ranges
        std::ranges::sort(nums);
        for(const auto& num : nums)
            cout << num << " ";
        cout << endl;
    }

    // 串联视图
    inline decltype(auto) func_07() {}

    // 过滤和变换
    inline decltype(auto) func_08() {
        std::vector vec{1, 2, 3, 4, 5};
        auto result = vec | std::views::transform([](int i){ return i * i; }) | std::views::take(3);
        cout << std::accumulate(result.begin(), result.end(), 0) << endl;
    }

    // 协程，我在考虑要不要说一下，这个内容比较多，但是和线程的内容又大同小异，基本思想是一样的
    // 这样吧，由于此知识的应用场景有IO，我在那部分笔记写这个知识点

    // 模块,这个需要在源文件里搞,此处就不介绍了

    // 三路比较符
    struct Point {
        int x{};
        int y{};

        auto operator<=>(const Point&) const = default;
    };

    inline decltype(auto) func_09() {
        constexpr Point p1{2, 6};
        constexpr Point p2{2, 5};
        if(p1 > p2) {
            cout << "win" << endl;
        }

    }

}


#endif //C_20_HPP
