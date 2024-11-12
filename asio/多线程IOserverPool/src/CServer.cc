//
// Created by whx on 24-11-8.
//

#include "../include/CServer.hpp"

#include <iostream>

CServer::CServer(boost::asio::io_context &ioc, const unsigned short port) : _port(port), _ioc(ioc),
                                                                            _acceptor(ioc, boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::any(), port))
{
    std::cout << R"(server start success, listen on port: )" << _port << std::endl;
    StartAccept();
}

void CServer::StartAccept()
{
    auto new_session = std::make_shared<CSession>(_ioc, this);
    _acceptor.async_accept(new_session->GetSocket(),
                           [this, new_session]<typename T0>(T0 &&PH1)
                           {
                               HandleAccept(new_session, std::forward<T0>(PH1));
                           });
}

void CServer::HandleAccept(const std::shared_ptr<CSession> &new_session, const boost::system::error_code &error)
{
    if (!error)
    {
        new_session->Start();
        _sessions.insert(std::make_pair(new_session->GetUuid(), new_session));
    }
    StartAccept();
}

void CServer::ClearSession(const std::string &uuid)
{
    _sessions.erase(uuid);
}
