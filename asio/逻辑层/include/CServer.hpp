//
// Created by whx on 24-11-8.
//

#ifndef CSERVER_HPP
#define CSERVER_HPP

#include "CSession.hpp"
#include <boost/asio.hpp>

class CServer {
public:
    explicit CServer(boost::asio::io_context &ioc, unsigned short port);

    void ClearSession(const std::string &uuid);

private:
    void StartAccept();

    void HandleAccept(const std::shared_ptr<CSession> &new_session, const boost::system::error_code &error);

private:
    unsigned short _port;
    boost::asio::io_context &_ioc;
    boost::asio::ip::tcp::acceptor _acceptor;
    std::map<std::string, std::shared_ptr<CSession>> _sessions;
};

#endif //CSERVER_HPP
