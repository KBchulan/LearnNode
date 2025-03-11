//
// Created by whx on 24-10-27.
//

/**
在一个项目中，全局范围内，某个类的实例有且仅有一个，通过这个唯一实例向其他模块提供数据的全局访问，这种模式就叫单例模式。
单例模式的典型应用就是任务队列
**/

#include <mutex>
#include <memory>
#include <iostream>

template <typename T>
concept Refer = requires(T t)
{
    {t.print_info()} -> std::same_as<void>;

};

template <typename T>
class Singlon {
public:
    ~Singlon() = default;
    explicit Singlon(const Singlon&) = delete;
    Singlon& operator=(const Singlon&) = delete;

    // 在懒汉的实例化时有几种方式,我们需要考虑多线程的安全性
    static std::shared_ptr<T> get_instance() {
        static std::once_flag o_flag;
        std::call_once(o_flag, [&] {
            instance = std::shared_ptr<T>(new T);
        });
        return instance;
    }

protected:
    Singlon() = default;

private:
    // 懒汉
    inline static std::shared_ptr<T> instance = nullptr;
};

class Son final: public Singlon<Son>{
public:
    void print_info() const {
        std::cout << R"(hello, singlon)" << std::endl;
    }
};

int main() {
    Son::get_instance()->print_info();
}


// 或者加锁
// static std::shared_ptr<T> _get_instance() {
//     if(!instance) {
//         std::lock_guard locker(_mutex);
//         instance = std::make_shared<T>(new T);
//     }
//     return instance;
// }

// 或者设置成原子,此处不做演示

// static std::mutex _mutex;
// 饿汉，区别在于是否直接创建出来
// inline static std::shared_ptr<T> instance_ = std::make_shared<T>();