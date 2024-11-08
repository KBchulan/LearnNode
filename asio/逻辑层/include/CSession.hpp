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

    // 会话的启动入口
    void Start();

    // 会话的关闭接口
    void Close();

    // 发送数据接口
    void Send(std::string msg, long msg_id);
    void Send(char *msg, long max_length, long msg_id);

    // socket外部获取接口
    boost::asio::ip::tcp::socket& GetSocket();

    // uuid外部获取接口
    std::string& GetUuid();

    // 自身的指针
    std::shared_ptr<CSession> SharedSelf();

private:
    void HandleRead(const boost::system::error_code& ec, size_t bytes_transferred, std::shared_ptr<CSession> shared_self);
    void HandleWrite(const boost::system::error_code& ec, std::shared_ptr<CSession> shared_self);

private:

};

#endif //CSESSION_HPP
