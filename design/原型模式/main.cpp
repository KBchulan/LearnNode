//
// Created by whx on 24-10-27.
//

/**
原型模式就是能够复制已有的对象，而又无需使代码依赖它们所属的类。
换种说法，就是通过已有对象克隆出另一个新的对象，并且克隆这个对象不需要使用构造函数。

与POD默认六种函数里的拷贝构造不同
克隆可能会在父类和子类之间进行，并且可能是动态的，
很明显通过父类的拷贝构造函数无法实现对子类对象的拷贝，
其实这就是一个多态，我们需要给父类提供一个克隆函数并且是一个虚函数。
**/
#include <memory>
#include <string>
#include <iostream>

class GermaSoldier {
public:
    virtual std::shared_ptr<GermaSoldier> clone() = 0;
    virtual std::string who_am_i() = 0;
    virtual ~GermaSoldier() = default;
};

class Solider66 final: public GermaSoldier {
public:
    std::shared_ptr<GermaSoldier> clone() override {
        return std::make_shared<Solider66>();
    }

    std::string who_am_i() override {
        return {"66"};
    }
};

class Solider67 final: public GermaSoldier {
public:
    std::shared_ptr<GermaSoldier> clone() override {
        return std::make_shared<Solider66>();
    }

    std::string who_am_i() override {
        return {"67"};
    }
};

int main() {
    // this is a son obj,but we can copy it to its father
    const std::shared_ptr<GermaSoldier> obj = std::make_shared<Solider66>();
    const std::shared_ptr<GermaSoldier> soldier = obj->clone();
    std::cout << soldier->who_am_i() << std::endl;
}

