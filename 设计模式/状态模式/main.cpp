//
// Created by whx on 24-11-1.
//

/**
状态模式就是在一个类的内部会有多种状态的变化，因为状态变化从而导致其行为的改变，
在类的外部看上去这个类就像是自身发生了改变一样
**/

#include <memory>
#include <string>
#include <iostream>

enum class name{杏菜, 佳树};

class WenShui;
class AbstractNode {
public:
    virtual ~AbstractNode() = default;

    virtual void working() = 0;
};

class JiaShu final : public AbstractNode {
public:
    void working() override {
        std::cout << "抱抱佳树" << '\n';
    }
};

class XiaoBa final : public AbstractNode {
public:
    void working() override {
        std::cout << "亲亲杏菜" << '\n';
    }
};

class WenShui {
public:
    explicit WenShui(){
        switch (_name) {
            case name::杏菜:
                _node = std::make_shared<XiaoBa>();
                break;
            case name::佳树:
                _node = std::make_shared<JiaShu>();
                break;
            default:
                break;
        }
    }

    void working() const {
        _node->working();
    }

protected:


private:
    name _name = name::佳树;
    std::shared_ptr<AbstractNode> _node;
};

int main() {
    std::shared_ptr<WenShui> wen_shui = std::make_shared<WenShui>();
    wen_shui->working();
}