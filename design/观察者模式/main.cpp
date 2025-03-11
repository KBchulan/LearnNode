//
// Created by whx on 24-11-1.
//

/**
观察者模式允许我们定义一种订阅机制，可在对象事件发生时通知所有的观察者对象，
使它们能够自动更新。观察者模式还有另外一个名字叫做“发布-订阅”模式
**/

#include <memory>
#include <vector>
#include <iostream>
#include <unordered_map>

class Person;
class SheTuan {
public:
    void add_member(const std::string& name, std::shared_ptr<Person> person) {
        _members[name] = std::move(person);
    }

    void decrease_member(const std::string& name) {
        if(_members.contains(name)) {
            _members.erase(name);
        }
    }

    void notify_all_member();

private:
    std::unordered_map<std::string, std::shared_ptr<Person>> _members;
};

class Person : public std::enable_shared_from_this<Person>{
public:
    explicit Person(std::string name, const std::shared_ptr<SheTuan>& she_tuan) : _name(std::move(name)), _she_tuan(she_tuan) {}

    void join() {
        _she_tuan->add_member(_name, shared_from_this());
    }

    void leave() const {
        _she_tuan->decrease_member(_name);
    }

    void updata() const {
        std::cout << _name << "收到" << '\n';
    }

private:
    std::string _name;
    std::shared_ptr<SheTuan> _she_tuan;
};

void SheTuan::notify_all_member() {
    for(const auto& [fir, sec] : _members) {
        sec->updata();
    }
}


int main() {
    auto she_tuan = std::make_shared<SheTuan>();
    const std::vector<std::string> names = {"八奈见杏菜", "温水佳树", "志喜屋"};
    std::vector<std::shared_ptr<Person>> members;
    members.reserve(names.size());

    for(auto& name : names) {
        members.emplace_back(std::make_shared<Person>(name, she_tuan));
        members.back()->join();
    }
    she_tuan->notify_all_member();
}

