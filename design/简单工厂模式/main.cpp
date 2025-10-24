//
// Created by whx on 24-10-27.
//

#include <memory>
#include <iostream>

/**
简单工厂模式可以提供一个类，统一的去生产对象（加工厂出来的产品）
steps:
1.创建一个新的类, 可以将这个类称之为工厂类。对于简单工厂模式来说，需要的工厂类只有一个。
2.在这个工厂类中添加一个公共的成员函数，通过这个函数来创建我们需要的对象，关于这个函数一般将其称之为工厂函数。
3.关于使用，首先创建一个工厂类对象，然后通过这个对象调用工厂函数，这样就可以生产出一个指定类型的实例对象了。
**/

/**********products*********/
class SheepSmile {
public:
    void transform() const {
        std::cout << R"(变为小羊)" << std::endl;
    }

    void ability() const {
        std::cout << R"(羊叫)" << std::endl;
    }
};

class LionSmile {
public:
    void transform() const {
        std::cout << R"(变为狮子)" << std::endl;
    }

    void ability() const {
        std::cout << R"(狮子叫)" << std::endl;
    }
};

class BatSmile {
public:
     void transform() const {
        std::cout << R"(变为蝙蝠)" << std::endl;
    }

    void ability() const{
        std::cout << R"(蝙蝠叫)" << std::endl;
    }
};
/**********products*********/

/**********factory*********/
enum class Type {
    Sheep,
    Lion,
    Bat
};

template<typename T>
class SmileFactory {
public:
    SmileFactory() = default;
    ~SmileFactory() = default;

    void* create_smile(const Type type) {
        switch (type) {
            case Type::Sheep:
                ptr = std::make_shared<SheepSmile>();
                break;
            case Type::Lion:
                ptr = std::make_shared<LionSmile>();
                break;
            case Type::Bat:
                ptr = std::make_shared<BatSmile>();
                break;
            default:
                break;
        }
        return ptr;
    }

private:
    std::shared_ptr<T> ptr = nullptr;
};
/**********factory*********/

int main(int argc, char **argv) {
    const auto factory = std::make_shared<SmileFactory<SheepSmile>>();
    [[maybe_unused]] const auto* obj = static_cast<SheepSmile *>(factory->create_smile(Type::Sheep));
    obj->ability();
    obj->transform();
}

/**
事实上这是一种比较愚蠢的写法,此处使用多态很显然更为优美
**/
namespace PROVE {
    class BaseSmile {
    public:
        virtual void transform() = 0;
        virtual void ability() = 0;
        virtual ~BaseSmile() = default;
    };

    class SheepSmile final : public BaseSmile {
    public:
        void transform() override {}
        void ability() override {}
    };

    enum Typee {sheep};
    class Factory {
    public:
        BaseSmile* create(const Typee type) {
            switch(type) {
                case Typee::sheep:
                    ptr = new SheepSmile();
                    break;
                default:
                    break;
            }
            return ptr;
        }

    private:
        BaseSmile* ptr = nullptr;
    };
}