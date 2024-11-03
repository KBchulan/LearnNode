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

#define HEAD_LENGTH 2

class MsgNode
{
    friend class CSession;
public:
    // send
    explicit MsgNode(const char *msg, const short max_len): _cur_len(0), _total_len(max_len + HEAD_LENGTH)
    {
        _data = new char[_total_len + 1]();
        int max_len_host = boost::asio::detail::socket_ops::host_to_network_short(max_len);
        memcpy(_data, &max_len_host, HEAD_LENGTH);
        memcpy(_data + HEAD_LENGTH, msg, max_len);
        _data[_total_len] = '\0';
    }

    // recv
    explicit MsgNode(const short max_len): _cur_len(0), _total_len(max_len) {
        _data = new char[_total_len + 1]();
    }

    ~MsgNode() {
        delete[] _data;
    }

    void Clear() {
        memset(_data, 0, _total_len);
        _cur_len = 0;
    }

private:
    int _cur_len;
    int _total_len;
    char* _data;
};

class CServer;
class CSession final :public std::enable_shared_from_this<CSession>{
public:
    // 上下文
    explicit CSession(boost::asio::io_context& ioc,
        CServer* server):
        _head_parse(false), _server(server), _data{}, _socket(ioc) {

        _recv_head_node = std::make_shared<MsgNode>(HEAD_LENGTH);

        //
        const boost::uuids::uuid a_uuid = boost::uuids::random_generator()();
        _uuid = to_string(a_uuid);
    }

    [[nodiscard]] boost::asio::ip::tcp::socket& Socket() {
        return _socket;
    }

    // 监听客户端的读和写
    void Start();

    // 发送数据
    void Send(char* msg, int max_length);

    [[nodiscard]] std::string& get_uuid() {
        return _uuid;
    }

private:
    void handle_read(const boost::system::error_code& error,
        size_t bytes_transferred, const std::shared_ptr<CSession>& _self_shared);

    void handle_write(const boost::system::error_code& error, const std::shared_ptr<CSession>& _self_shared);

    enum {max_length = 2048};

private:
    bool _head_parse;
    CServer* _server;
    std::string _uuid;
    std::mutex _send_lock;
    char _data[max_length];
    boost::asio::ip::tcp::socket _socket;
    std::shared_ptr<MsgNode> _recv_msg_node;
    std::shared_ptr<MsgNode> _recv_head_node;
    std::queue<std::shared_ptr<MsgNode>> _send_que;
};

#endif //CSESSION_HPP
