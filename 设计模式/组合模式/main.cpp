//
// Created by whx on 24-10-28.
//

/**
能将多个对象组成一个树状结构，用以描述部分----整体的层次关系，使得用户对单个对象和组合对象的使用具有一致性，这样的结构性设计模式叫做组合模式
链表我们有写过这种节点类的组合，这个其实只是换成类了
**/

#include <vector>
#include <memory>
#include <string>
#include <utility>
#include <iostream>

class AbstractTeam {
public:
    explicit AbstractTeam(const std::string &name):_name(std::move(name)){}
    virtual ~AbstractTeam() = default;

    void setParent(const std::shared_ptr<AbstractTeam>& parent) {
        _parent = parent;
    }

    [[nodiscard]] std::shared_ptr<AbstractTeam> getParent() const {
        return _parent;
    }

    [[nodiscard]] std::string getName() const {
        return _name;
    }

    virtual bool hasSon() {
        return false;
    }

    virtual void addSon(const std::shared_ptr<AbstractTeam>& sonNode){}
    virtual void removeSon(__gnu_cxx::__normal_iterator<const std::shared_ptr<AbstractTeam> *, std::vector<std::shared_ptr<AbstractTeam>>> sonNode){}
    virtual void fight() = 0;
    virtual void display() = 0;

protected:
    std::string _name;
    std::shared_ptr<AbstractTeam> _parent;
};

class LeafeTeam final: public AbstractTeam {
public:
    using AbstractTeam::AbstractTeam;

    void fight() override {
        std::cout << "i am" << _name << ",";
        std::cout << R"(i am fighting)" << std::endl;
    }

    void display() override {
        std::cout << _parent->getName() << " " << _name << std::endl;
    }

};

class ManagerTeam final: public AbstractTeam {
public:
    using AbstractTeam::AbstractTeam;

    bool hasSon() override {
        return true;
    }

    void addSon(const std::shared_ptr<AbstractTeam> &sonNode) override {
        sonNode->setParent(std::shared_ptr<AbstractTeam>(this));
        _children.push_back(sonNode);
    }

    void removeSon(const __gnu_cxx::__normal_iterator<const std::shared_ptr<AbstractTeam> *, std::vector<std::shared_ptr<AbstractTeam>>> sonNode) override {
        _children.erase(sonNode);
    }

    void fight() override {
        std::cout << "fight" << std::endl;
    }

    void display() override {

    }

private:
    std::vector<std::shared_ptr<AbstractTeam>> _children;
};

int main() {

}