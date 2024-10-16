#include "../include/config.hpp"

namespace NORMAL
{
    using namespace std;

    // 默认参数,声明时从右往左入栈
    void f(int, int, int = 9);
    void f(int, int = 8, int);
    void f(int = 7, int , int);

    void f(int a,int b,int c) {
        cout << a << " " << b << " " << c <<endl;
    }

    // 默认实参，在virtual修饰的函数中，默认实参根据类的静态类型确定
    struct Base
    {
        virtual void func(int a = 5)
        {
            std::cout << "Base" << a << std::endl;
        }

        Base() = default;
        virtual ~Base() = default;
    };

    struct Child : Base
    {
        void func(int a = 7)
        {
            std::cout << "Bhild" << a << std::endl;
        }

        using Base::Base;
        ~Child() override = default;
    };

    void print_info()
    {
        std::unique_ptr<Base> ua = std::make_unique<Child>();
        // 输出：Bhild5
        ua->func();
    }
}

namespace PATTERNPTR
{
    using namespace std;
}

int main()
{
    std::ios::sync_with_stdio(NULL);
    std::cin.tie(nullptr);std::cout.tie(nullptr);
    {
        using namespace NORMAL;
        print_info();
        f();
    }
}