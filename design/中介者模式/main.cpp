//
// Created by whx on 24-11-1.
//

/**
中介者模式可以减少对象之间混乱无序的依赖关系，从而使其耦合松散，限制对象之间的直接交互，迫使它们通过一个中介者对象进行合作
网状结构--->以中介者为中心的放射形结构，即对象之间不再直接发挥作用，而是借助中介者起作用
**/

#include <memory>
#include <iostream>
#include <unordered_map>

class Country {
public:
    Country() = default;
    virtual ~Country() = default;

    virtual void declare(std::string msg, std::string country) = 0;

    virtual void set_message(std::string msg) = 0;

    virtual std::string get_name() = 0;
};

class AbstractMid {
public:
    virtual ~AbstractMid() = default;

    void add_member(const std::shared_ptr<Country>& country) {
        _country_map[country->get_name()] = country;
    }

    // 中间函数
    virtual void declare(std::string msg, std::shared_ptr<Country> country, std::string name) = 0;

protected:
    std::unordered_map<std::string, std::shared_ptr<Country>> _country_map;
};

class WorldGovt final : public AbstractMid {
public:
    void declare(const std::string msg, const std::shared_ptr<Country> country, const std::string name) override {
        if(_country_map.contains(name)) {
            _country_map[name]->set_message(msg + "come from" + country->get_name());
        }
    }
};

// 接下来可以实例化一定country，借助中间的WorldGovt进行通信即可