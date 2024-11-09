//
// Created by whx on 24-11-8.
//

#ifndef MSGNODE_HPP
#define MSGNODE_HPP

#include <iostream>
#include <boost/asio.hpp>

class MsgNode {
public:
    MsgNode &operator=(const MsgNode &) = delete;
    MsgNode &&operator=(const MsgNode &&) = delete;

    explicit MsgNode(const long max_len): _total_len(max_len) {
        _data = new char[_total_len + 1]();
        _data[_total_len] = '\0';
    }

    virtual ~MsgNode() {
        delete []_data;
    }

    void Clear() {
        memset(_data, 0, _total_len);
        _cur_len = 0;
    }

    char *_data{};
    long _cur_len{};
    long _total_len{};

};

class RecvNode final : public MsgNode {
public:
    RecvNode(long max_len, long msg_id);
    ~RecvNode() override{
        std::cout << "RecvNode " << _msg_id << " deleted" << std::endl;
    }

private:
    long _msg_id{};
};

class SendNode final : public MsgNode {
public:
    SendNode(const char *msg, long max_len, long msg_id);
    ~SendNode() override{
        std::cout << "SendNode " << _msg_id << " deleted" << std::endl;
    }

private:
    long _msg_id{};
};

#endif //MSGNODE_HPP
