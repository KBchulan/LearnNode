//
// Created by whx on 24-11-1.
//

/**
策略模式需要我们定义一系列的算法，并且将每种算法都放入到独立的类中，在实际操作的时候使这些算法对象可以相互替换
**/

#include <memory>
#include <iostream>

enum class GirlName {
    八奈见杏菜,
    温水佳树,
    志喜屋
};

class Girl {
public:
    virtual ~Girl() = default;

    virtual void faver() = 0;
};

class XiaoBa final : public Girl {
public:
    void faver() override {
        std::cout << "eat" << '\n';
    }
};

class JiaShu final : public Girl {
public:
    void faver() override {
        std::cout << "wenshui" << '\n';
    }
};

class ZhiXi final : public Girl {
public:
    void faver() override {
        std::cout << "sleep" << '\n';
    }
};

class WenShui {
public:
    void aver() {
        switch (_girl_name) {
            case GirlName::八奈见杏菜:
                _girl = std::make_shared<XiaoBa>();
                break;
            case GirlName::温水佳树:
                _girl = std::make_shared<JiaShu>();
                break;
            case GirlName::志喜屋:
                _girl = std::make_shared<ZhiXi>();
                break;
            default:
                break;
        }
        _girl->faver();
    }

private:
    GirlName _girl_name = GirlName::八奈见杏菜;
    std::shared_ptr<Girl> _girl;
};

int main() {
    const auto wen_shui = std::make_shared<WenShui>();
    wen_shui->aver();
}