//
// Created by whx on 24-11-1.
//

/**
在不破坏封装性的前提下，捕获一个对象的内部状态，并在该对象之外保存这个状态，
这样以后在需要的时候就可以将该对象恢复到原先保存的状态
**/

#include <vector>
#include <memory>
#include <chrono>
#include <utility>
#include <iostream>
#include <unordered_map>

class Girl {
public:
    explicit Girl(std::string name) : _name(std::move(name)){}

    [[nodiscard]] std::string get_name() {
        return _name;
    }

private:
    std::string _name;
};

class Thing {
public:
    Thing() = default;
    Thing(const std::chrono::duration<int, std::ratio<60 * 60 * 24>> day_some, std::shared_ptr<Girl> girl , std::string thing) {
        _now += day_some;
        _girl = std::move(girl);
        _thing = std::move(thing);
    }

    void get_info() const {
        const time_t time = std::chrono::system_clock::to_time_t(_now);
        std::cout << R"(我将在)" << std::ctime(&time) << R"(和)" << _girl->get_name() << _thing << '\n';
    }

private:
    std::chrono::system_clock::time_point _now = std::chrono::system_clock::now();
    std::shared_ptr<Girl> _girl;
    std::string _thing = std::string();
};

class Recoter {
public:
    void add_arrange(const int idx, const std::shared_ptr<Thing> &arrange) {
        _arrange[idx] = arrange;
    }

    std::shared_ptr<Thing> get_arrange(const int idx) {
        if(_arrange.contains(idx)) {
            return _arrange[idx];
        }
        return std::make_shared<Thing>();
    }

private:
    std::unordered_map<int, std::shared_ptr<Thing>> _arrange;
};

int main() {
    std::vector<std::string> girl_names = {"八奈见杏菜", "温水佳树", "志喜屋"};
    std::vector<std::string> thing_names = {"接吻", "牵手", "睡觉"};

    std::vector<std::shared_ptr<Girl>> girl_list;
    girl_list.reserve(girl_names.size());

    // 女角
    for(const auto& girl_name : girl_names) {
        girl_list.push_back(std::make_shared<Girl>(girl_name));
    }

    int idx = 0;
    const std::shared_ptr<Recoter> recoter = std::make_shared<Recoter>();

    for(auto& girl : girl_list) {
        for(auto &thing : thing_names) {
            std::chrono::duration<int, std::ratio<60 * 60 * 24>> day(idx);
            recoter->add_arrange(idx++, std::make_shared<Thing>(day, girl, thing));
        }
    }

    for (auto i = 0; i < idx; i++) {
        recoter->get_arrange(i)->get_info();
    }

}
