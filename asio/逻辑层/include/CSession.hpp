//
// Created by whx on 24-11-8.
//

#ifndef CSESSION_HPP
#define CSESSION_HPP

#include <queue>
#include <iostream>
#include <config.hpp>
#include <MsgNode.hpp>
#include <boost/asio.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

class CServer;
class CSession final : public std::enable_shared_from_this<CSession> {
public:
    explicit CSession(boost::asio::io_context &ioc, CServer *server);

    ~CSession();

    // 会话的启动入口
    void Start();

    // 会话的关闭接口
    void Close();

    // 发送数据接口
    void Send(const std::string& msg, long msg_id);
    void Send(char *msg, long max_length, long msg_id);

    // socket外部获取接口
    boost::asio::ip::tcp::socket& GetSocket();

    // uuid外部获取接口
    std::string& GetUuid();

    // 自身的指针
    std::shared_ptr<CSession> SharedSelf();

private:
    void HandleRead(const boost::system::error_code& ec, size_t bytes_transferred, const std::shared_ptr<CSession>& shared_self);
    void HandleWrite(const boost::system::error_code& ec, const std::shared_ptr<CSession>& shared_self);

private:
    bool _b_close;                                      // 会话是否关闭
    std::string _uuid;                                  // 会话的id
    bool _b_head_parse;                                 // 头部是否处理完成
    std::mutex _send_lock;                              // 为HandleWrite的回调队列加锁处理
    char _data[MAX_LENGTH];                             // 数据缓冲区
    std::shared_ptr<CServer> _server;                   // 会话归属的服务
    boost::asio::ip::tcp::socket _socket;               // 通信的socket
    std::shared_ptr<MsgNode> _recv_head_node;           // 收到的头部（TL）
    std::shared_ptr<RecvNode> _recv_msg_node;           // 收到的数据体
    std::queue<std::shared_ptr<SendNode>> _send_queue;  // 发送节点队列
};

#endif //CSESSION_HPP
