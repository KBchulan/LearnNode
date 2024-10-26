//
// Created by whx on 24-10-26.
//
// we prefer use async_read_some in read process
// instead of async_send in send process

#ifndef SESSION_HPP
#define SESSION_HPP

#include <queue>
#include <memory>
#include <iostream>
#include <boost/asio.hpp>

// 此处我们接收的字符串设置为定长
constexpr int RECVSIZE = 1024;

class MsgNode final {
public:
    // 发送节点
    explicit MsgNode(const char *msg, const int total_len) : _total_len(total_len), _cur_len(0) {
        _msg = new char[total_len];
        memcpy(_msg, msg, total_len);
    }

    // 接收节点
    explicit MsgNode(const int total_len) : _total_len(total_len), _cur_len(0) {
        _msg = new char[total_len];
    }

    ~MsgNode() {
        delete []_msg;
    }

public:
    int _total_len;    // 总共处理的长度
    int _cur_len;      // 当前处理长度
    char *_msg;
};

class Session final{
public:
    explicit Session(const std::shared_ptr<boost::asio::ip::tcp::socket>& socket);

    void Connect(const boost::asio::ip::tcp::endpoint& ep) const;

    void WriteCallbackErr(const boost::system::error_code& ec, std::size_t bytes_transferred,
        const std::shared_ptr<MsgNode>& );

    void WriteToSocketErr(const std::string& buf);

    void WriteCallback(const boost::system::error_code& ec, std::size_t bytes_transferred);

    void WriteToSocket(const std::string& buf);

    void WriteAllCallback(const boost::system::error_code& ec, std::size_t bytes_transferred);

    void WriteAllToSocket(const std::string& buf);

    void ReadFromSocket();

    void ReadCallBack(const boost::system::error_code& ec, std::size_t bytes_transferred);

private:
    std::queue<std::shared_ptr<MsgNode>> _send_queue;
    std::shared_ptr<boost::asio::ip::tcp::socket> _socket;
    std::shared_ptr<MsgNode> _send_node;
    bool _send_pending;

    std::shared_ptr<MsgNode> _recv_node;
    bool _recv_pending;
};

#endif //SESSION_HPP
