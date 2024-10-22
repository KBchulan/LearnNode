//
// Created by whx on 24-10-22.
//

#ifndef C_17_HPP
#define C_17_HPP

#include <tuple>
#include <variant>
#include <iostream>
#include <optional>
#include <functional>


namespace CXX_17 {
    using namespace std;

    // 折叠表达式，一般有4种形式，左折就是参数从左边开始运算，右折就是参数从右边开始计算

    // 一元左折(右折只是参数和...换个位置)
    // 格式：(... 运算符 一包参数)
    // 计算方式：（参数1 运算符 参数2）运算符....

    // 二元左折(右折只是初始化方向变一下)
    // 格式:(init 运算符 ... 运算符 一包参数)
    // 计算方式:(init 运算符 参数1) 运算符....

    template <typename... Args>
    auto sub_val_left(Args&&... args) {
        return (... - args);
    }

    template <typename... Args>
    auto sub_val_right(Args&&... args) {
        return (args - ...);
    }

    template <typename... Args>
    auto sub_val_left_two(Args&&... args) {
        return (200 - ... - args);
    }

    template <typename... Args>
    auto sub_val_right_two(Args&&... args) {
        return (args - ... - 200);
    }

    inline void fold_workspace() {
        cout << sub_val_left(10, 20, 30, 40) << endl;
        cout << sub_val_right(10, 20, 30, 40) << endl;
        cout << sub_val_left_two(10, 20, 30, 40) << endl;
        cout << sub_val_right_two(10, 20, 30, 40) << endl;
    }

    // 类模板类型的推导
    // 以往类在实例化时要求是必须显式声明，现在可以通过推导得到了
    template <typename T>
    class A {
    public:
        A(T,T){}
    };

    inline void class_auto_judge() {
        // 原来这样写是不对的
        const auto y = new A(5, 6);
        delete y;
    }

    // 在模板中，我们通常给定typename，并在实例化时显式声明，但是你是否这样想过，auto
    // 能自己推导类型，这里为什么没有这样写过，其实是可以的，但作者认为用处不是很大
    template <auto T>
    auto func() {cout << T << endl;}

    inline void class_result_auto() {
        func<100>();
        func<2.0>();
    }

    // 编译期constexpr if或者constexpr的lambda表达式
    // 众所周知，lambda函数体内，不能有汇编，不能有goto。不能有label，
    // try，不能分配内存等等等等,但是，这都是在运行期间才会报错，
    // 而带上我们的constexpr后(对函数也行)，编译阶段就会进行运算并报错，可以提高运行效率
    template<bool ok>
    constexpr decltype(auto) func_02() {
        if constexpr(ok == true) {
            cout << "ok" << endl;
        }
        else {
            cout << "not ok" << endl;
        }
    }

    inline void constexpr_lambda_workspace() {
        func_02<true>();
        func_02<false>();

        constexpr decltype(auto) callback = [](int a, int b) {
            return a + b;
        };

        auto aaa = [callback]
        <typename T0, typename T1>(T0 && PH1, T1 && PH2)
        { return callback(std::forward<T0>(PH1), std::forward<T1>(PH2)); };

        cout << aaa(2, 6) << endl;
    }

    // inline这个其实可以粗略的说一下
    // 在头文件我们若是定义一个函数或者变量或者命名空间，若是包含于多个源文件，很容易出现
    // 重定义的问题，原因是其global属性，inline则可以削弱这个属性，使其变为weak，
    // 这样就解决了编译问题，但是多次定义会使编译器不知道用什么，因此还是自己定义的好
    // 最好用的一条还得是在类内初始化静态成员变量
    // 不做演示，上文说的比较清晰了已经

    // 结构化绑定
    // 在c++11中，获取tuple中的元素需要使用get<>()或者tie<>函数，从而将元素转换为
    // tie<>()的左值集合，而我们的结构化绑定就是为了简化这个操作
    // 还有for循环的范围遍历，若是对pair对象，可以直接使用结构化绑定，这个比较常用，
    // 就不写了
    struct Student {
        std::string name;
        size_t age;
    };

    inline Student get_student() { return {"luxuanyu", 18};}

    inline decltype(auto) c11_func() {
        const auto student = std::make_tuple(std::string("lilei"), 26, std::string("man"));
        std::string name;
        size_t age;
        std::string gender;
        std::tie(name, age, gender) = student;
        cout << name << " " << age << " " << gender << endl;
    }

    inline decltype(auto) c17_func() {
        const auto student = std::make_tuple(std::string("lilei"), 26, std::string("man"));
        // 下面这一行就是
        auto [name, age, gender] = student;
        cout << name << " " << age << " " << gender << endl;
    }

     inline decltype(auto) workspace_07() {
        c11_func();
        c17_func();
    }

    // if，switch初始化,比较鸡肋，差不多是下面这样，在一定程度上确实可以避免命名重复，
    // 但不是很有必要
    inline decltype(auto) workspace_08() {
        // c11
        std::unordered_map<int, int> a = {
            {2,3},{5, 6}
        };
        const auto it = a.find(2);
        if(it != a.end()) { }

        // c17
        std::unordered_map<int, int> b = {
            {2,3},{5, 6}
        };
        if(const auto iter = b.find(5);
            iter != b.end()){}
    }

    // 简化命名空间，跟using搭配起来还是有点用的
    namespace AAA{namespace BBB{namespace CCC{inline void func(){}}}}
    // --->
    namespace AAA::BBB::CCC{inline void funct(){}}

    // c++17还优化了求值顺序
    /**
    后缀表达式从左到右求值，包括函数调用和成员选择表达式；
    赋值表达式从右向左求值，包括复合赋值；
    从左到右计算移位操作符的操作数。
     **/
    // 除了赋值，都是从左到右
    inline int a(){return std::puts("a");}
    inline int b(){return std::puts("b");}
    inline int c(){return std::puts("c");}
    inline decltype(auto) z(int, int, int) {}

    inline int workspace_09() {
        std::unordered_map<int, int> aaaa;
        // 插入的是{0,0}还是{0,1}，这就是顺序问题,这个是赋值，所以是0
        aaaa[0] = static_cast<int>(aaaa.size());
        std::cout << aaaa[0] << std::endl;

        z(a(), b(), c());
        return a() + b() + c();
    }

    // 一些属性
    // 还记得我们c++14中的[[deprecated]]吗，可以修饰某个东西不受人欢迎，我们的17
    // 有了几个新的这种东西,咱们只说有用的

    // 表示修饰的内容不可忽略,即在调用时必须保存返回值（非void)
    [[nodiscard]] inline decltype(auto) func_10(){cout << "hello,world" << endl;}

    // 加入你使用的是Clion，经常看到这样的报错：这个变量只是定义，并未使用，
    // 但我们学习过程中根本没有使用的必要，我们就可以加上[[maybe_unused]]

    inline decltype(auto) workspace_10() {
        int x = 5;
        [[maybe_unused]] int y = 6;
        func_10();
    }

    // 库相关
    // std::variant,和qt同一样，有qt基础的小伙伴应该一眼知道是干什么的这个
    // 这个的构造六种POD默认的都支持,可以说是更高级的union，非常有用
    // 介绍一下构造和取值，其实成员函数非常少，且见名知意

    // 特点：varient只能包含其拥有的类型之一
    // 自动管理和析构，资源管理方便，可以通过get或者visit（推荐）访问
    inline decltype(auto) variant_func() {
        std::variant<int, std::string, double> var = 5;
        std::cout << std::get<int>(var) << endl;
        var = "xiaoming";
        std::cout << std::get<std::string>(var) << endl;
        var = 1.3;
        std::cout << std::get<double>(var) << endl;

        // 访问存储的数据
        try {
            cout << std::get<int>(var) << endl;
        }
        catch (const std::bad_variant_access&) {
            cout << "Error: The current variant does not hold an int." << endl;
        }

        // visit的访问更为安全且高效，并且支持多态,不管类型，都能打印出来
        std::visit([](auto& arg) {
            cout << arg << endl;
        }, var);

        // 下面说一下这个的高级用法
        // holds_alternative检测类型，返回一个布尔值
        cout << std::holds_alternative<double>(var) << endl;

        // get_if还是访问元素，不过不会抛出异常,返回指针，因此若没有查找到，则返回nullptr
        cout << *std::get_if<double>(&var) << endl;
    }

    // std::optional
    // 其实说白了就是一种数据类型，可以包含int，double，自定义类型等等等等的东西，只是多了一个
    // 未赋值的安全处理机制，如果我们养成了初始化的好习惯。我认为这是没有必要的
    // 引入这个东西的原因，我认为是c++底层缺少None（python里的）这种东西
    // 表示一个可能存在或不存在的值，有一个安全的方式来表示缺失的数值:std::nullopt
    inline decltype(auto) optional_func() {
        constexpr bool is_even = true;
        constexpr std::optional<int> value = is_even ? std::optional<int>(128) : std::nullopt;
        cout << value.value() << endl;

        // 比如找vector中第一个为0的索引
        std::vector<int> aa = {1, 2, 3, 4, 0, 6};
        auto callback = [nums = aa]() ->std::optional<int> {
            for(auto i = 0; i < nums.size(); i++) {
                if(nums[i] == 0)
                    return i;
            }
            return std::nullopt;
        };
        cout << callback().value() << endl;

        // 构造
        std::optional<int> val0 = std::nullopt; // 或者列表初始化
        val0.emplace(5);
        cout << *val0 << endl;

        constexpr std::optional<int> val1 = std::make_optional(6);
        cout << *val1 << endl;

        constexpr auto val2 = std::optional<int>(7);
        cout << *val2 << endl;

        constexpr auto val3 = std::optional(8.8);
        cout << *val3 << endl;

        // has_value判断是否有值
        cout << val3.has_value() << endl;

        // 获取值，用*或者.value()，不展示了

    }

    // std::tuple

    // std::as_const



}

#endif //C_17_HPP
