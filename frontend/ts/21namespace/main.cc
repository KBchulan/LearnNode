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

    auto newFunc = func();
    newFunc();
}