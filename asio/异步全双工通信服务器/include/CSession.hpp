//
// Created by whx on 24-10-28.
//

#ifndef CSESSION_HPP
#define CSESSION_HPP

#include <queue>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

class MsgNode
{
    friend class CSession;
public:
    explicit MsgNode(const char *msg, const int max_len) {
        _data = new char[max_len];
        memcpy(_data, msg, max_len);
    }

    ~MsgNode() {
        delete[] _data;
    }
private:
    int _cur_len{};
    int _max_len{};
    char* _data;
};

class CServer;
class CSession final : public std::enable_shared_from_this<CSession>{
public:
    // 上下文
    explicit CSession(boost::asio::io_context& ioc,
        CServer* server):
        _server(server), _data{}, _socket(ioc) {
        const boost::uuids::uuid a_uuid = boost::uuids::random_generator()();
        _uuid = to_string(a_uuid);
    }

    [[nodiscard]] boost::asio::ip::tcp::socket& Socket() {
        return _socket;
    }

    // 监听客户端的读和写
    void Start();

    void Send(char* msg, int max_length);

    [[nodiscard]] std::string& get_uuid() {
        return _uuid;
    }

private:
    void handle_read(const boost::system::error_code& error,
        size_t bytes_transferred, const std::shared_ptr<CSession>& _self_shared);

    void handle_write(const boost::system::error_code& error, const std::shared_ptr<CSession>& _self_shared);

    enum {max_length = 1024};

private:
    CServer* _server;
    std::string _uuid;
    char _data[max_length];
    boost::asio::ip::tcp::socket _socket;
    std::queue<std::shared_ptr<MsgNode>> _send_que;
    std::mutex _send_lock;
};


#endif //CSESSION_HPP
