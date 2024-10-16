#ifndef SMART_PTR_HPP
#define SMART_PTR_HPP

// 智能指针知识点比较少，大多在于应用，此处我给出一个单例的实现
// 可以在看一下CRTP的实现

#include "type.hpp"

#include <mutex>
#include <atomic>
#include <memory>
#include <functional>
#include <unordered_map>

template <typename T>
class SmartPtr {
public:
    SmartPtr() {
        get_ins_pool[InsID::Atomic] = GetInstance_atomic();
        get_ins_pool[InsID::OneFlag] = GetInstance_flag();
    }
    virtual ~SmartPtr() noexcept = default;


private:
    static std::shared_ptr<T> GetInstance_flag()
    {
        static std::once_flag flag;
        std::call_once(flag,[&]() {
            instance = std::shared_ptr<T>(new T);
        });
        return instance;
    }

    static std::shared_ptr<T> GetInstance_atomic() {

    }

private:
    inline static std::shared_ptr<T> instance = nullptr;
    std::function<std::shared_ptr<T>> GetInstance;

private:
    InsID ins_id;
    std::unordered_map<InsID, std::shared_ptr<T>()> ins_pool;
    std::unordered_map<InsID, std::function<std::shared_ptr<T>()>> get_ins_pool;

protected:
    SmartPtr(const SmartPtr<T> &) = delete;
    SmartPtr(const SmartPtr<T> &&) = delete;
    SmartPtr &operator=(const SmartPtr<T> &) = delete;
    SmartPtr &operator=(const SmartPtr<T> &&) = delete;

};


#endif //SMART_PTR_HPP
