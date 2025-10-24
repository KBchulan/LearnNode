//
// Created by whx on 24-10-30.
//

/**
动态的给一个对象绑定额外的属性
装饰模式也可以称之为封装模式，所谓的封装就是在原有行为之上进行拓展，并不会改变该行为
**/

#include <string>
#include <memory>
#include <iostream>

class Solider {
public:
    Solider() = default;
    explicit Solider(std::string name):_name(std::move(name)){}
    virtual ~Solider() = default;

    virtual void fight() = 0;

    [[nodiscard]] std::string getName() const {
        return _name;
    }

private:
    std::string _name;
};

class SuperMan final: public Solider {
public:
    using Solider::Solider;

    void fight() override {
        std::cout << R"(i am superman)" << '\n';
    }
};

// 开始装饰
class Fruit : virtual public Solider {
public:
    explicit Fruit(std::string name) : Solider(std::move(name)) {}
    virtual ~Fruit() = default;

    void setSolider(const std::shared_ptr<Solider> &solider) {
        this->_solider = solider;
    }

protected:
    std::shared_ptr<Solider> _solider;
};

class Apple final: public Fruit {
public:
    explicit Apple(std::string name) : Fruit(std::move(name)) {}
    ~Apple() override = default;

    void fight() override {
        _solider->fight();
        std::cout << R"(i am apple)" << '\n';
    }
};

int main() {
    const std::shared_ptr<Solider> super_man = std::make_shared<SuperMan>("ba");
    const std::shared_ptr<Fruit> apple = std::make_shared<Apple>("apple");
    apple->setSolider(super_man);
    apple->fight();
}