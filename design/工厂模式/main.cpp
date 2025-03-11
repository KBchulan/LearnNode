//
// Created by whx on 24-10-27.
//

#include <memory>

/**
我们其实可以看到简单工厂模式是有很大的缺陷的，
如果我们想要增加果实的种类，则一定会需要改写
工厂基类的函数，这很显然违背了我们的封闭原则，
因此我们引入工厂模式来解决此问题，即将工厂设
置为基类，并为不同应用场景进行扩展，
很容易可以看出，每个子类工厂生产的东西是一样的
**/

// 先定义果实
class BaseSmile {
public:
    virtual void ability() = 0;
    virtual ~BaseSmile() = default;
};

class SheepSmile final: public BaseSmile {
public:
    using BaseSmile::BaseSmile;
    void ability() override {}
};

class TigerSmile final: public BaseSmile {
public:
    using BaseSmile::BaseSmile;
    void ability() override {}
};

class BaseFactory {
public:
    virtual BaseSmile* create() = 0;
    virtual ~BaseFactory() = default;
};

class SheepFactory final: public BaseFactory {
public:
    BaseSmile* create() override{return new SheepSmile;}
};

int main() {
    std::shared_ptr<BaseFactory> sheep_factory = std::make_shared<SheepFactory>();
}
