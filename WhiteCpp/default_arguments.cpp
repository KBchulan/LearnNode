#include <memory>
#include <iostream>

namespace DEFAULT_ARGUMENTS
{
    using namespace std;

    namespace PROBLEM_1
    {
        // 默认实参必须位于函数的右侧吗，
        // 并不是的，我们只需要在当前声明的形参之前给予其右侧实参默认值即可
        // 类似一个入栈操作，上述要求在同一个作用域
        void f(int, int, int = 10);
        void f(int, int = 9, int);
        void f(int = 8, int, int);
        void f(int a, int b, int c)
        {
            cout << a << " " << b << " " << c << endl;
        }

        // 例外情况，如果某个形参是参数包，那么其不纳入形参队列
        template <typename... Args>
        decltype(auto) f_(int n = 0, Args... args)
        {
            cout << n << endl;
            cout << (... + args) << endl;
        }
    }

    namespace PROBLEM_2
    {
        // 类的默认实参
        // 我们先看下面的程序，你认为这三个函数定义哪个有问题
        class A
        {
        public:
            void f(int = 3);
            void g(int i, int j = 5);
            explicit A(int arg);
        };

        void A::f(int i = 4) {}
        void A::g(int i = 5, int j) {}
        A::A(int arg = 8) {}

        // 我们会发现f,A两个都报错了，下面解释一下
        // 非模板类允许类外定义的默认实参和类内的相结合，共同组成默认实参表
        // 注意我们说的是结合，而非覆盖，同时，若是类外的默认实参使类内的构造函数
        // 出现赋值和移动，那么我们可以说程序非良构，是会报错的
        // 同时，类模板的默认构造函数必须在类内给予初始值
        // 那么，对于类，我们默认在类内进行默认参数赋值是比较好的选择
    }

    namespace PROBLEM_3
    {

    }

}

int main(int argc, char **argv)
{
    std::ios::sync_with_stdio(NULL);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    std::cout << "----------+++----------" << std::endl;
    {
        using namespace DEFAULT_ARGUMENTS;
        // PROBLEM_1::f();
        // PROBLEM_1::f_(10, 20);
        // std::shared_ptr<PROBLEM_2::A> a = std::make_shared<PROBLEM_2::A>();
    }
    std::cout << "----------+++----------" << std::endl;
}