#include <memory>
#include <iostream>

struct Message
{
    explicit Message(int data) : data(data){}
    int data{};
};

std::ostream &operator<<(std::ostream &os, const std::shared_ptr<Message> &msg)
{
    os << msg->data;
    return os;
}

int main()
{
    std::shared_ptr<Message> msg = std::make_shared<Message>(6);
    std::cout << msg;
}