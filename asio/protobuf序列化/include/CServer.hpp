//
// Created by whx on 24-10-28.
//

#ifndef CSERVER_HPP
#define CSERVER_HPP

#include "CSession.hpp"

#include <boost/asio.hpp>

class CServer {
public:
    explicit CServer(boost::asio::io_context& ioc, unsigned short port);

    void clear_session(const std::string& uuid);

private:
    void start_accept();

    void handle_accept(const std::shared_ptr<CSession>& new_session, const boost::system::error_code& error);

private:
    boost::asio::io_context& _ioc;
    boost::asio::ip::tcp::acceptor _acceptor;
    std::map<std::string, std::shared_ptr<CSession>> _sessions;
};

#endif //CSERVER_HPP
