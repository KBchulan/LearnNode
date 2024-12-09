#ifndef SINGLETON_H
#define SINGLETON_H

#include <mutex>
#include <memory>
#include <iostream>

template <typename T>
class Singleton
{
public:
    virtual ~Singleton()
    {
        std::cerr << "Singleton base has been destructed\n";
    }

    static std::shared_ptr<T> GetInstance() noexcept
    {
        static std::once_flag g_flag;
        std::call_once(g_flag, []() -> void
        {
            _instance = std::shared_ptr<T>(new T); 
        });
        return _instance;
    }

    void PrintAddress() const noexcept
    {
        std::cout << _instance.get() << '\n';
    }

public:
    Singleton(const Singleton &) = delete;
    Singleton &operator=(const Singleton &) = delete;

protected:
    Singleton() = default;

private:
    inline static std::shared_ptr<T> _instance = nullptr;
};

#endif // !SINGLETON_H