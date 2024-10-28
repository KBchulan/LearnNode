//
// Created by whx on 24-10-28.
//

/**
将一个类的接口转换成用户希望的另一个接口，使不兼容的对象能够相互配合并一起工作，这种模式就叫适配器模式

一般这么一种情况：我们原来写了一些函数或者类，然后客户端用户期望拿到一个接口，为了提高代码复用，我们
设计一个适配器来调用，那么其实就是：原函数---适配器--->目标接口
**/

#include <memory>
#include <iostream>

class Source {
public:
    void func() const { // NOLINT(*-convert-member-functions-to-static)
        std::cout << R"(aaaa)" << std::endl;
    }
};

class Target {
public:
    virtual void func() {
        std::cout << R"(new function)" << std::endl;
    }
    virtual ~Target() = default;
};

class Adapter final: public Target {
public:
    explicit Adapter(std::unique_ptr<Source> source): source_(std::move(source)){}

    void func() override {
        // 这样我们就实现了原有接口的调用
        source_->func();
        // else
    }

private:
    std::unique_ptr<Source> source_;
};