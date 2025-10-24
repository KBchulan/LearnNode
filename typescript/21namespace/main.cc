#include <iostream>
#include <functional>

int main()
{
    auto func = []() -> std::function<void()>
    {
        std::uint32_t test = 1;
        auto myFunc = [&test]() -> void
        {
            std::cout << test << '\n';
        };
        return myFunc;
    };

    auto newFunc = func();  // 此时test的生命周期已经结束，就访问不到了，因为c++并没有闭包这个概念
    newFunc();
}