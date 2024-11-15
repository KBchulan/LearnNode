#ifndef CSERVER_HPP
#define CSERVER_HPP

#include <map>
#include <mutex>
#include <memory>
#include <boost/asio.hpp>

class CSession;
class CServer
{
public:
    explicit CServer(boost::asio::io_context &ioc, short port);

    ~CServer();

    // 清除某一个会话
    void ClearSession(std::string);

private:
    // 接收的回调
    void HandleAccept(std::shared_ptr<CSession>, const boost::system::error_code &error);

    // 开始接收
    void StartAccept();

private:
    short _port;
    std::mutex _mutex;
    boost::asio::io_context &_ioc;
    boost::asio::ip::tcp::acceptor _acceptor;
    std::map<std::string, std::shared_ptr<CSession>> _sessions;
};

#endif // !CSERVER_HPP