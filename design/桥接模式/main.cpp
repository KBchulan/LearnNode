//
// Created by whx on 24-10-30.
//

/**
将抽象部分和它的实现部分分离，使它们可以独立的变化，这种处理模式就是桥接模式
即继承改为聚合
**/

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <iostream>

struct Person {
    Person(const std::string& name, const std::string &job, const std::string &ability
        , const std::string &reward, const std::string &beizhu=std::string("")) {
        this->_name = name;
        this->_job = job;
        this->_ability = ability;
        this->_reward = reward;
        this->_beizhu = beizhu;
    }
    std::string _name;    // 名字
    std::string _job;     // 职责
    std::string _ability; // 能力
    std::string _reward;  // 赏金
    std::string _beizhu;  // 备注
};

// 舰队
class AbstractTeam {
public:
    explicit AbstractTeam(std::string name):_team_name(std::move(name)) {}
    virtual ~AbstractTeam() = default;

    [[nodiscard]] std::string get_name()const {
        return _team_name;
    }

    void addMember(const std::shared_ptr<Person>& person) {
        _info_map.insert(std::make_pair(person->_name, person));
    }

    void show() {
        std::cout << _team_name << '\n';
        for(const auto& [fir, sec] : _info_map) {
            std::cout << "name:" << sec->_name << " "
            << "job:" << sec->_job << " " << "ability:" << sec->_ability << " " << "reward:" << sec->_reward
            << "beizhu:" << sec->_beizhu << '\n';
        }
    }

    virtual void target() = 0;

protected:
    std::string _team_name;
    std::map<std::string, std::shared_ptr<Person>> _info_map;
};

class CaoMaoTeam final: public AbstractTeam {
public:
    using AbstractTeam::AbstractTeam;

    void target() override {
        std::cout << "caomao" << '\n';
    }
};

// 这里写一下船只即可，不做展示了
class AbstractShip {

};