//
// Created by whx on 24-11-9.
//

#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include <mutex>
#include <memory>
#include <iostream>

template<typename T>
class Singleton {
public:
    Singleton(const Singleton &) = delete;
    Singleton &operator=(const Singleton &) = delete;

    ~Singleton() {
        std::cout << R"(This singleton has been destructed!)" << '\n';
    }

    static std::shared_ptr<T> GetInstance() {
        static std::once_flag g_flag;
        std::call_once(g_flag, [&] {
            // ReSharper disable once CppSmartPointerVsMakeFunction
            _instance = std::shared_ptr<T>(new T);
        });
        return _instance;
    }

    void PrintAddress() {
        std::cout << _instance.get() << std::endl;
    }

private:
    inline static std::shared_ptr<T> _instance = nullptr;

protected:
    Singleton() = default;

};

#endif //SINGLETON_HPP
