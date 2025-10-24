//
// Created by whx on 24-10-21.
//

#ifndef WIDE_LAMBDA_HPP
#define WIDE_LAMBDA_HPP

#include <future>
#include <memory>
#include <iostream>

namespace WIDELAMBDA {
    using namespace std;
    // c++14以前，lambda表达式的形参需要被声明为具体类型，但是c++14放宽了这一点，允许将其
    // 声明为auto,同时泛型的lambda函数遵循模板推导的原则

    struct unname_lambda {
        template <typename T, typename U>
        auto operator()(T x, U y){ return x + y;}
    };

    inline void workspace_01() {
        auto lambda_01 = [](auto x, auto y){return x + y;};
        cout << lambda_01(2, 5) << endl;
        auto lambda_02 = unname_lambda{};
        cout << lambda_02(3, 6) << endl;
    }

    // 捕获部分使用表达式
    // c++11中捕获列表是值传递和引用，因此不能是move-only类型的变量
    // c++14中lambda成员可以用任意被捕获表达式初始化，即我们的广义捕获，可以在捕获列表增加并初始化
    // 新的变量，即不需要外部有一个相同名字的变量
    // 整体来说：移动捕获，自定义字面量
    // 效果，内部不能影响外部，外部只有在捕获时影响内部

    inline void workspace_02() {
        auto lambda_03 = [x = 5, y = 6]{cout << x + y << endl;};
        lambda_03();
        auto x = 1;
        auto lambda_04 = [&r = x, x = x * 10]() {
            ++r;
            cout << x << endl;      // 这里我们就可以发现一个事情，即lambda函数体的闭包域只与自己
                                    // 捕获列表有关，而不影响外部作用域
            cout << r + x << endl;
        };
        cout << x << endl;
        lambda_04();

        // auto lambda_05 = [x, y = 6]{}; ok
        std::shared_ptr<int> ptr(new int(10));
        auto lambda_05 = [ptra = std::move(ptr)] {
            cout << *ptra << endl;
        };

        lambda_05();
    }

    // 函数返回类型推导
    // c++11允许lambda通过return推导返回类型，c++14的一般函数也可以通过return推导类型，
    // 而且不需要满足return expression

    // 下面这个函数是举的反例
    // inline auto Corrent(double x) {
    //     if(x == 1)
    //         Corrent(x - 1) + x;
    //     return x;
    // }

    inline void workspace_03() {
        // 我们可以看到这样调用是不行的，因为多个return语句应当被推导成一种类型，但是递归语句在前
        // 就会导致auto->auto,这显然是不行的，在多个返回类型的auto推导中，必须前向定义
        // Corrent(3);
    }

    // 另一种类型推导
    // c++11有auto和decltype两种推导，但是auto常推导出非引用，auto&&常为引用,而且非指针和volatile无法推导出const修饰
    // c++14增加了decltype(auto)的语法，即decltype的实参不需要显式制定，可以通过auto推导
    inline void workspacr_04() {
        constexpr int x = 0;
        [[maybe_unused]] auto x1 = x;    // int
        [[maybe_unused]] constexpr decltype(auto) x2 = x;    // int
        auto& y = x;
        [[maybe_unused]] decltype(auto) y1 = y;
        int&& z = 0;
        [[maybe_unused]] decltype(auto) z1 = std::move(z);
    }

    // 也可以用decltype(auto)替换auto

    // constexpr的函数限制，没有必要学，感兴趣可以搜一下

    // 变量模板
    // c++11是类型模板或函数模板，14则有变量模板
    template <typename T>
    constexpr T pi = T(3.141593653);

    // 特化
    template <>
    constexpr const auto &pi<const char *> = "pi";

    inline void workspace_05() {
        cout << pi<int> << endl;
        cout << pi<double> << endl;
        cout << pi<float> << endl;
        cout << pi<const char *> << endl;
    }

    // 聚合类成员初始化
    // 一句话，14允许聚合类在初始化列表时使用default member initializer
    struct CXX_14_initializer {
        int x{};
        int y = 6;
    };

    inline void workspace_06() {
        CXX_14_initializer a = {6};         // 列表初始化从上到下,只会给x赋值
        cout << a.x << " " << a.y << endl;
    }

    // 二进制字面量
    // 数字可以由0b或0B+二进制数，我的评价是就不是个好东西（可能对性能有一点帮助）

    // 数字分位符,就是方便人看，这个好
    inline void workspace_07() {
        constexpr auto inter_01 = 100'0000;
        cout << inter_01 << endl;
        constexpr auto inter_02 = 0b1111;
        cout << inter_02 << endl;
    }

    // deprecated属性
    // 标记不推荐使用，只是让用户知道这个是不受人欢迎的,在编译时也可能会出现警告
    [[deprecated]] int f();
    [[deprecated("g is not a useful func, please use h")]]
    void g();
    void h();

    inline void workspace_08() {
        // 按照下面两个调用会报错
        // f();
        // g();
    }

    // 共享的互斥体和锁,感觉不如一个共享资源一个锁来的实在
    inline std::shared_future<int> a;

    // 元函数的别名
    // c++11的元函数用于查询一个给定类型是否具有某种特征，或者转换给定类型的某种特征，从而得到另一个类型。后一种元函数通过成员类型type来返回转换后的类型，当它们用在模板中时，必须使用typename关键字，这会增加代码的长度。
    template <class T>
    auto get_type_object(T&) -> typename std::remove_cv<typename std::remove_reference<T>::type>::type{return T();}

    // 可以看出是十分不优美的写法,在14中，我们可以这样写
    template <class T>
    auto get_type_object_new(T&) -> std::remove_cv_t<std::remove_reference_t<T>>{return T{};}

    /**
     *在C++14，拥有类型别名的元函数包括：remove_const、remove_volatile、remove_cv、add_const、add_volatile、
     *add_cv、remove_reference、add_lvalue_reference、add_rvalue_reference、make_signed、make_unsigned、
     *remove_extent、remove_all_extents、remove_pointer、add_pointer、aligned_storage、aligned_union、
     *decay、enable_if、conditional、common_type、underlying_type、result_of、tuple_element
     **/

    // 关联容器的异构查找
    // 标准库中允许通过键去查找并返回一个数值，查找只允许指定参数
    // 14以后可以通过其他类型进行查找，如std::set<std::string>>可以通过const char*
    // 或者任意一个通过operator重载与std::string进行关联的类型
    // 不举例子了，后续写代码用到了会回来更新链接

    // 标准自定义字面量
    // 用户可以定义字面量后缀，此处我们看一下14定义的一些字面量
    /**
    "s"，用于创建各种std::basic_string类型。
    "h"、"min"、"s"、"ms"、"us"、"ns"，用于创建相应的std::chrono::duration时间间隔。
    "if"、"i"、"il"用于创建相应的 std::complex<float>、 std::complex<double> 和 std::complex<long double> 复数类型。
     ***/

    inline void workspace_09() {
        auto str = "hello,world"s;
        [[maybe_unused]] auto dur_01 = 60s;
        [[maybe_unused]] auto dur_02 = 60h;
        [[maybe_unused]] auto dur_03 = 60ms;
    }

    // 11的元组允许我们通过索引查询，现在可以用类型查询了
    inline std::tuple<std::string, std::string, int> t("aaa", "bbb", 100);
    inline int i = get<int>(t);
    inline int j = get<2>(t);

    // 小知识点，看下名字就知道是干啥的了
    // 类的继承里有一个is_final，判断能否被继承
    // begin/end--->>>rbegin/rend


}

#endif //WIDE_LAMBDA_HPP
