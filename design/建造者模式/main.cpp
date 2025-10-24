//
// Created by whx on 24-10-27.
//

/**
工厂模式关注的是如何创建对象，强调的是对象的类型；而制造者模式关注的是如何一步一步构建一个复杂对象，强调的是构建过程。
工厂模式适合简单的对象创建，而制造者模式则适合需要复杂构建步骤的场景。

整体而言，建造者模式其实就是复杂事物构造拆分化，成为简单步骤的综合，且由于分离，会更加可控
**/

#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <unordered_map>

class SunnyShip {
public:
    void add_parts(const std::string& part) {
        parts.emplace_back(part);
    }

    void get_parts() const {
        for (const auto& part : parts) {
            std::cout << part << "\n";
        }
    }

private:
    std::vector<std::string> parts;
};

class HeavenShip {
public:
    void add_parts(const std::string& str1, const std::string& str2) {
        parts[str1] = str2;
    }

    void get_part() const {
        for(const auto& [first, second] : parts) {
            std::cout << first << " " << second << "\n";
        }
    }

private:
    std::unordered_map<std::string, std::string> parts;
};

// abstract builder
class BaseBuilder {
public:
    virtual void reset() = 0;
    virtual void add_body() = 0;
    virtual void add_weapon() = 0;
    virtual void add_engine() = 0;
    virtual void add_interdir() = 0;
    virtual void get_ship() = 0;
    virtual ~BaseBuilder() = default;
};

class SunnyBuilder final: public BaseBuilder {
public:
    SunnyBuilder() {
        reset();
    }

    void reset() override {
        sunny_ship_ = std::make_unique<SunnyShip>();
    }

    void add_body() override {
        sunny_ship_->add_parts("sunbody");
    }

    void add_weapon() override {
        sunny_ship_->add_parts("sunweapon");
    }

    void add_engine() override {
        sunny_ship_->add_parts("sunengine");
    }

    void add_interdir() override {
        sunny_ship_->add_parts("suninterdir");
    }

    void get_ship() override{
        sunny_ship_->get_parts();
    }

private:
    std::unique_ptr<SunnyShip> sunny_ship_;
};

class HeavenBuild final: public BaseBuilder {
public:
    HeavenBuild() {
        reset();
    }

    void reset() override {
        heaven_ship_ = std::make_unique<HeavenShip>();
    }

    void add_body() override {
        heaven_ship_->add_parts("hea", "heabody");
    }

    void add_weapon() override {
        heaven_ship_->add_parts("hea", "heaweapon");
    }

    void add_engine() override {
        heaven_ship_->add_parts("hea", "heaengine");
    }

    void add_interdir() override {
        heaven_ship_->add_parts("hea", "heainterdir");
    }

    void get_ship() override {
        heaven_ship_->get_part();
    }

private:
    std::unique_ptr<HeavenShip> heaven_ship_;
};

class Director {
public:
    void set_build(std::unique_ptr<BaseBuilder> builder) {
        builder_ = std::move(builder);
    }

    void build_simple() {
        builder_->add_body();
        builder_->add_engine();
    }

    void build_pro_one() {
        build_simple();
        builder_->add_weapon();
    }

    void build_pro_two() {
        build_pro_one();
        builder_->add_interdir();
    }

    [[nodiscard]] BaseBuilder* get_builder() const {
        return builder_.get();
    }

private:
    std::unique_ptr<BaseBuilder> builder_;
};

int main() {
    std::unique_ptr<BaseBuilder> builder = std::make_unique<SunnyBuilder>();
    const std::unique_ptr<Director> director = std::make_unique<Director>();
    director->set_build(std::move(builder));

    director->build_simple();
    director->get_builder()->get_ship();

    return 0;
}

