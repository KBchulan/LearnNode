//
// Created by whx on 24-10-30.
//

/**
为其他对象提供一种代理，从而控制对象访问
整体还是通过其他方式实现同一目标
**/

#include <memory>
#include <iostream>

class Communication {
public:
    virtual void communication() = 0;
    virtual ~Communication() = default;
};

class Speaker final: public Communication {
public:
    void communication() override {
        std::cout << R"(i am speaking)" << std::endl;
    }
};

class Midsession final: public Communication {
public:
    Midsession() {
        is_start = true;
        _speaker = std::make_shared<Speaker>();
    }

    void communication() override {
        if(is_start) {
            _speaker->communication();
        }
    }

private:
    bool is_start = false;
    std::shared_ptr<Speaker> _speaker;
};

int main() {
    const std::shared_ptr<Communication> speaker = std::make_shared<Speaker>();
    speaker->communication();

    const std::shared_ptr<Communication> mid = std::make_shared<Midsession>();
    mid->communication();
}