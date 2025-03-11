//
// Created by whx on 24-10-31.
//

/**
提供一种解决方案使其能够顺序访问一个集合对象中的各个元素，而又不暴露该集合底层的表现形式（列表、栈、树、图等）
，这种行为设计模式就叫迭代器模式
**/

#include <vector>
#include <string>
#include <memory>
#include <iostream>

struct Node {
    explicit Node(std::string name) : _name(std::move(name)) {}

    std::string _name;
    std::shared_ptr<Node> _next;
    std::shared_ptr<Node> _prev;
};

class Iterator;
class ForwardIterator;
class ReverseIterator;
// 双向链表
class MyList : public std::enable_shared_from_this<MyList>{
public:
    [[nodiscard]] int getCount() const {
        return _count;
    }

    std::shared_ptr<Node> head() {
        return _head;
    }

    std::shared_ptr<Node> tail() {
        return _tail;
    }

    std::shared_ptr<Node> insert(const std::shared_ptr<Node>& item, const std::string& data) {
        std::shared_ptr<Node> node;
        if (item == _head) {
            node = pushFront(data);
        }
        else {
            node = std::make_shared<Node>(data);
            node->_next = item;
            node->_prev = item->_prev;
            item->_prev->_next = node;
            item->_prev = node;
        }
        _count++;
        return node;
    }

    std::shared_ptr<Node> pushFront(const std::string& data) {
        const auto node = std::make_shared<Node>(data);

        if (_head == nullptr) {
            _head = _tail = node;
        }
        else {
            node->_next = _head;
            _head->_prev = node;
            _head = node;
        }
        _count++;
        return node;
    }

    std::shared_ptr<Node> pushBack(const std::string& data) {
        const auto node = std::make_shared<Node>(data);

        if (_tail == nullptr) {
            _head = _tail = node;
        }
        else {
            _tail->_next = node;
            node->_prev = _tail;
            _tail = node;
        }
        _count++;
        return node;
    }

    std::shared_ptr<Iterator> get_iterator(bool isReverse = false);

private:
    std::shared_ptr<Node> _head;
    std::shared_ptr<Node> _tail;
    int _count = 0;
};

class Iterator {
public:
    Iterator() = default;
    explicit Iterator(std::shared_ptr<MyList> my_list): _my_list(std::move(my_list)) {}
    virtual ~Iterator() = default;

    std::shared_ptr<Node> current() {
        return _current;
    }

    virtual std::shared_ptr<Node> first() = 0;
    virtual std::shared_ptr<Node> next() = 0;
    virtual bool isDone() = 0;

protected:
    std::shared_ptr<MyList> _my_list;
    std::shared_ptr<Node> _current;
};

class ForwardIterator final : public Iterator{
public:
    using Iterator::Iterator;

    std::shared_ptr<Node> first() override {
        _current = _my_list->head();
        return _current;
    }

    std::shared_ptr<Node> next() override {
        _current = _current->_next;
        return _current;
    }

    bool isDone() override {
        return _current == _my_list->tail()->_next;
    }
};

class ReverseIterator final : public Iterator{
public:
    using Iterator::Iterator;

    std::shared_ptr<Node> first() override {
        _current = _my_list->tail();
        return _current;
    }

    std::shared_ptr<Node> next() override {
        _current = _current->_prev;
        return _current;
    }

    bool isDone() override {
        return _current == _my_list->head()->_prev;
    }
};

std::shared_ptr<Iterator> MyList::get_iterator(const bool isReverse) {
    if(isReverse)
        return std::make_shared<ReverseIterator>(shared_from_this());
    return std::make_shared<ForwardIterator>(shared_from_this());
}


int main() {
    std::ios::sync_with_stdio(NULL);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    std::vector<std::string> nameList{
        "烬", "奎因", "杰克", "福兹·弗", "X·德雷克",
        "黑色玛利亚", "笹木", "润媞", "佩吉万",
        "一美", "二牙", "三鬼", "四鬼", "五鬼",
        "六鬼", "七鬼", "八茶", "九忍","十鬼"
    };

    const std::shared_ptr<MyList> my_list = std::make_shared<MyList>();
    for (const auto& name : nameList) {
        my_list->pushBack(name);
    }

    std::shared_ptr<Iterator> it = my_list->get_iterator(true);

    for(auto begin = it->first(); !it->isDone(); it->next()) {
        std::cout << it->current()->_name << " ";
    }
}