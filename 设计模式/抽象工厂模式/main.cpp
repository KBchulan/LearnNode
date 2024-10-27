//
// Created by whx on 24-10-27.
//

/**
在工厂模式中，不同工厂的生成组件是一个确定属性，那么若是此属性也不确定呢，
那么组件是不是也应当进行抽象化，但是最终的工厂目的足够明确即可
抽象工厂模式在需要管理一组相关或依赖的对象创建时，能够提供灵活的解决方案
接下来我们以造船为例，有1,2,3等级的船，其中船包括船体，动力，武器
**/

#include <iostream>
#include <memory>
#include <utility>

class ShipBody {
public:
    virtual ~ShipBody() = default;
    virtual void get_material() = 0;
};

class WoodBody final : public ShipBody {
public:
    void get_material() override {
        std::cout << R"(wood)" << std::endl;
    }
};

class StoneBody final : public ShipBody {
public:
    void get_material() override {
        std::cout << R"(stone)" << std::endl;
    }
};

class IronBody final : public ShipBody {
public:
    void get_material() override {
        std::cout << R"(iron)" << std::endl;
    }
};

class ShipEngine {
public:
    virtual ~ShipEngine() = default;
    virtual void get_engine() = 0;
};

class HumanEngine final : public ShipEngine {
public:
    void get_engine() override {
        std::cout << R"(human)" << std::endl;
    }
};

class DieselEngine final : public ShipEngine {
public:
    void get_engine() override {
        std::cout << R"(diesel)" << std::endl;
    }
};

class ElectricEngine final : public ShipEngine {
public:
    void get_engine() override {
        std::cout << R"(electric)" << std::endl;
    }
};

class ShipWeapon {
public:
    virtual ~ShipWeapon() = default;
    virtual void get_weapon() = 0;
};

class GunWeapon final : public ShipWeapon {
public:
    void get_weapon() override {
        std::cout << R"(gun)" << std::endl;
    }
};

class CannonWeapon final : public ShipWeapon {
public:
    void get_weapon() override {
        std::cout << R"(cannon)" << std::endl;
    }
};

class TorpedoWeapon final : public ShipWeapon {
public:
    void get_weapon() override {
        std::cout << R"(torpedo)" << std::endl;
    }
};

class Ship {
public:
    Ship(std::shared_ptr<ShipBody> ship_body, std::shared_ptr<ShipEngine> ship_engine,
         std::shared_ptr<ShipWeapon> ship_weapon)
        : ship_body_(std::move(ship_body)),
          ship_engine_(std::move(ship_engine)),
          ship_weapon_(std::move(ship_weapon)) {}

    void get_info() const {
        ship_body_->get_material();
        ship_engine_->get_engine();
        ship_weapon_->get_weapon();
    }

private:
    std::shared_ptr<ShipBody> ship_body_;
    std::shared_ptr<ShipEngine> ship_engine_;
    std::shared_ptr<ShipWeapon> ship_weapon_;
};

class Factory {
public:
    virtual ~Factory() = default;
    virtual std::shared_ptr<Ship> create_ship() = 0; // 改为返回 std::shared_ptr
};

class OneFactory final : public Factory {
public:
    std::shared_ptr<Ship> create_ship() override {
        return std::make_shared<Ship>(
            std::make_shared<WoodBody>(),
            std::make_shared<HumanEngine>(),
            std::make_shared<GunWeapon>()
        );
    }
};

class TwoFactory final : public Factory {
public:
    std::shared_ptr<Ship> create_ship() override {
        return std::make_shared<Ship>(
            std::make_shared<StoneBody>(),
            std::make_shared<DieselEngine>(),
            std::make_shared<CannonWeapon>()
        );
    }
};

class ThreeFactory final : public Factory {
public:
    std::shared_ptr<Ship> create_ship() override {
        return std::make_shared<Ship>(
            std::make_shared<IronBody>(),
            std::make_shared<ElectricEngine>(),
            std::make_shared<TorpedoWeapon>()
        );
    }
};

int main() {
    const std::shared_ptr<Factory> factory = std::make_shared<OneFactory>();
    const auto new_ship = factory->create_ship();
    new_ship->get_info();
    return 0;
}
