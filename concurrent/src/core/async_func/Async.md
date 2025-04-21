# C++异步编程：async、future和promise

## 1. 基础概念

### 1.1 什么是异步编程

异步编程是一种编程模式，允许任务在后台执行而不阻塞主线程。在C++中，`<future>`头文件提供了实现异步操作的工具。

### 1.2 C++异步编程的核心组件

- **std::future**: 表示将来某个时间点能获取的值
- **std::promise**: 存储一个值，可以在未来被异步地获取
- **std::async**: 启动异步任务并返回future对象
- **std::packaged_task**: 封装可调用对象，使其运行结果被存储到future中

## 2. 源码分析

### 2.1 std::future的实现

`std::future`是一个模板类，它的核心是通过共享状态(`shared state`)实现的。

```cpp
template<typename R>
class future {
private:
    std::shared_ptr<_Future_impl<R>> _M_impl; // 内部实现

public:
    // 获取结果，如果结果未就绪则阻塞
    R get() {
        if (!_M_impl)
            throw future_error(future_errc::no_state);
        return _M_impl->get(); // 委托给实现类
    }

    // 检查future是否有效
    bool valid() const noexcept { return static_cast<bool>(_M_impl); }

    // 等待结果就绪
    void wait() const {
        if (!_M_impl)
            throw future_error(future_errc::no_state);
        _M_impl->wait();
    }

    // 等待结果，但有超时
    template<typename _Clock, typename _Duration>
    future_status wait_until(
        const chrono::time_point<_Clock, _Duration>& __abs_time) const {
        if (!_M_impl)
            throw future_error(future_errc::no_state);
        return _M_impl->wait_until(__abs_time);
    }

    // 等待结果，但最多等待指定时间
    template<typename _Rep, typename _Period>
    future_status wait_for(
        const chrono::duration<_Rep, _Period>& __rel_time) const {
        if (!_M_impl)
            throw future_error(future_errc::no_state);
        return _M_impl->wait_for(__rel_time);
    }
};
```

### 2.2 std::promise的实现

`std::promise`负责设置未来将被`std::future`读取的值。

```cpp
template<typename R>
class promise {
private:
    std::shared_ptr<_Promise_impl<R>> _M_impl; // 内部实现

public:
    promise() : _M_impl(std::make_shared<_Promise_impl<R>>()) {}

    // 设置值
    void set_value(const R& __r) {
        if (!_M_impl)
            throw future_error(future_errc::no_state);
        _M_impl->set_value(__r);
    }

    void set_value(R&& __r) {
        if (!_M_impl)
            throw future_error(future_errc::no_state);
        _M_impl->set_value(std::move(__r));
    }

    // 设置异常
    void set_exception(std::exception_ptr __p) {
        if (!_M_impl)
            throw future_error(future_errc::no_state);
        _M_impl->set_exception(__p);
    }

    // 获取关联的future
    future<R> get_future() {
        if (!_M_impl)
            throw future_error(future_errc::no_state);
        return _M_impl->get_future();
    }
};
```

### 2.3 std::async的实现

`std::async`是一个函数模板，它启动异步任务并返回future对象。

```cpp
template<typename _Fn, typename... _Args>
future<typename result_of<_Fn(_Args...)>::type>
async(launch __policy, _Fn&& __fn, _Args&&... __args) {
    using _Res = typename result_of<_Fn(_Args...)>::type;

    // 创建共享状态
    auto __state = std::make_shared<_Async_state<_Res>>();
    auto __future = __state->get_future();

    if (__policy == launch::async) {
        // 启动新线程
        try {
            std::thread __t(&_Async_state<_Res>::_M_run, __state,
                          std::forward<_Fn>(__fn),
                          std::forward<_Args>(__args)...);
            __t.detach();
        } catch (...) {
            __state->set_exception(std::current_exception());
        }
    } else if (__policy == launch::deferred) {
        // 延迟执行
        __state->_M_set_deferred(__fn, __args...);
    } else {
        // 由实现决定
        if (/*系统决定使用新线程*/) {
            try {
                std::thread __t(&_Async_state<_Res>::_M_run, __state,
                              std::forward<_Fn>(__fn),
                              std::forward<_Args>(__args)...);
                __t.detach();
            } catch (...) {
                __state->set_exception(std::current_exception());
            }
        } else {
            __state->_M_set_deferred(__fn, __args...);
        }
    }

    return __future;
}
```

## 3. 底层实现机制

### 3.1 共享状态(Shared State)

future和promise之间通过共享状态进行通信：

```cpp
template<typename R>
class _Shared_state {
private:
    std::mutex _M_mutex;
    std::condition_variable _M_cond;
    bool _M_ready = false;
    std::variant<std::monostate, R, std::exception_ptr> _M_result;

public:
    void set_value(const R& __r) {
        {
            std::lock_guard<std::mutex> __lock(_M_mutex);
            if (_M_ready)
                throw future_error(future_errc::promise_already_satisfied);
            _M_result = __r;
            _M_ready = true;
        }
        _M_cond.notify_all();
    }

    void set_exception(std::exception_ptr __p) {
        {
            std::lock_guard<std::mutex> __lock(_M_mutex);
            if (_M_ready)
                throw future_error(future_errc::promise_already_satisfied);
            _M_result = __p;
            _M_ready = true;
        }
        _M_cond.notify_all();
    }

    R get() {
        std::unique_lock<std::mutex> __lock(_M_mutex);
        _M_cond.wait(__lock, [this] { return _M_ready; });

        if (std::holds_alternative<std::exception_ptr>(_M_result))
            std::rethrow_exception(std::get<std::exception_ptr>(_M_result));

        return std::get<R>(_M_result);
    }

    void wait() {
        std::unique_lock<std::mutex> __lock(_M_mutex);
        _M_cond.wait(__lock, [this] { return _M_ready; });
    }
};
```

### 3.2 线程同步机制

- **互斥锁(std::mutex)**: 保护共享状态的访问
- **条件变量(std::condition_variable)**: 用于线程间的通知
- **原子操作**: 用于无锁状态检查

## 4. 上层应用实例

### 4.1 基本用法

```cpp
#include <iostream>
#include <future>
#include <thread>
#include <chrono>

int compute_value() {
    std::this_thread::sleep_for(std::chrono::seconds(2)); // 模拟耗时操作
    return 42;
}

int main() {
    // 使用async启动异步任务
    std::future<int> result = std::async(std::launch::async, compute_value);

    std::cout << "正在计算..." << std::endl;

    // 获取结果（如果尚未就绪，会阻塞）
    int value = result.get();
    std::cout << "结果: " << value << std::endl;

    return 0;
}
```

### 4.2 使用promise和future进行线程间通信

```cpp
#include <iostream>
#include <future>
#include <thread>

void producer(std::promise<int> promise) {
    // 模拟耗时计算
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 设置promise的值
    promise.set_value(42);
}

void consumer(std::future<int> future) {
    // 等待并获取值
    int value = future.get();
    std::cout << "收到值: " << value << std::endl;
}

int main() {
    // 创建promise和future
    std::promise<int> promise;
    std::future<int> future = promise.get_future();

    // 创建生产者和消费者线程
    std::thread producer_thread(producer, std::move(promise));
    std::thread consumer_thread(consumer, std::move(future));

    // 等待线程完成
    producer_thread.join();
    consumer_thread.join();

    return 0;
}
```

### 4.3 异常处理

```cpp
#include <iostream>
#include <future>
#include <stdexcept>

void task_with_exception() {
    throw std::runtime_error("发生了错误");
}

int main() {
    // 使用async启动可能抛出异常的任务
    std::future<void> result = std::async(std::launch::async, task_with_exception);

    try {
        // 尝试获取结果
        result.get();
    } catch (const std::exception& e) {
        std::cout << "捕获到异常: " << e.what() << std::endl;
    }

    return 0;
}
```

### 4.4 使用packaged_task封装可调用对象

```cpp
#include <iostream>
#include <future>
#include <thread>
#include <functional>

int multiply(int a, int b) {
    return a * b;
}

int main() {
    // 创建packaged_task
    std::packaged_task<int(int,int)> task(multiply);

    // 获取future
    std::future<int> result = task.get_future();

    // 在新线程中执行任务
    std::thread task_thread(std::move(task), 6, 7);

    // 等待结果
    int value = result.get();
    std::cout << "结果: " << value << std::endl;

    task_thread.join();
    return 0;
}
```

## 5. 高级特性

### 5.1 std::shared_future

允许多个线程等待同一个结果：

```cpp
#include <iostream>
#include <future>
#include <thread>
#include <vector>

void wait_for_value(std::shared_future<int> future, int id) {
    int value = future.get(); // 多个线程可以调用get()
    std::cout << "线程 " << id << " 收到值: " << value << std::endl;
}

int main() {
    // 创建promise
    std::promise<int> promise;

    // 获取shared_future
    std::shared_future<int> shared_future = promise.get_future().share();

    // 创建多个等待线程
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(wait_for_value, shared_future, i);
    }

    // 设置值，所有线程都会收到
    promise.set_value(42);

    // 等待所有线程完成
    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
```

### 5.2 等待超时

```cpp
#include <iostream>
#include <future>
#include <chrono>

int slow_operation() {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    return 42;
}

int main() {
    auto future = std::async(std::launch::async, slow_operation);

    // 尝试等待1秒
    auto status = future.wait_for(std::chrono::seconds(1));

    if (status == std::future_status::ready) {
        std::cout << "结果已就绪: " << future.get() << std::endl;
    } else if (status == std::future_status::timeout) {
        std::cout << "等待超时，继续其他工作..." << std::endl;
        // 稍后获取结果
        std::cout << "最终结果: " << future.get() << std::endl;
    } else if (status == std::future_status::deferred) {
        std::cout << "操作是延迟的，调用get()会执行它" << std::endl;
        std::cout << "结果: " << future.get() << std::endl;
    }

    return 0;
}
```

## 6. 性能考量与最佳实践

- **避免过度线程化**: 不要创建比系统核心数多太多的线程
- **适当使用延迟执行**: 对于可能不需要执行的任务，使用`std::launch::deferred`
- **减少线程间数据共享**: 尽量减少多线程间共享状态的修改
- **注意异常处理**: 确保在异步任务中抛出的异常能被正确捕获

## 7. 实现限制与注意事项

- **线程创建开销**: 每次使用`std::async`可能创建新线程，有一定开销
- **延迟执行的缺点**: 使用`std::launch::deferred`时，如果不调用`get()`或`wait()`则任务永远不会执行
- **资源泄漏风险**: 未调用`get()`的future在析构时可能导致程序挂起

## 8. 与其他并发技术的比较

- **相比于直接的thread管理**: 更高级别的抽象，更容易使用
- **相比于线程池**: 没有线程复用机制，但使用更简单
- **相比于callback**: 代码更清晰，避免回调地狱
