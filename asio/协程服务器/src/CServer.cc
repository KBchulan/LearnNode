#include "../include/CServer.hpp"
#include "../include/CSession.hpp"
#include "../include/AsioIOServicePool.hpp"

#include <iostream>

CServer::CServer(boost::asio::io_context &ioc, short port) : _port(port), _ioc(ioc),
                                                             _acceptor(_ioc, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), _port))

{
    std::cout << R"(CServer success on port :)" << _port << '\n';
    StartAccept();
}

CServer::~CServer()
{
    std::cout << R"(CServer desstruct)" << std::endl;
}

void CServer::ClearSession(std::string uuid)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _sessions.erase(uuid);
}

void CServer::StartAccept()
{
    auto &io_context = AsioIOServicePool::GetInstance().GetIOContext();
    std::shared_ptr<CSession> new_session = std::make_shared<CSession>(io_context, this);

    _acceptor.async_accept(new_session->GetSocket(),
                           [this, new_session](const boost::system::error_code &error)
                           {
                               HandleAccept(new_session, error);
                           });
}

void CServer::HandleAccept(std::shared_ptr<CSession> new_session, const boost::system::error_code &error)
{
    if(!error)
    {
        new_session->Start();

        std::lock_guard<std::mutex> lock(_mutex);
        _sessions.insert(std::make_pair(new_session->GetUuid(), new_session));
    }
    else
    {
        std::cerr << R"(Session accept failed, error is)" << error.message() << '\n';
    }
    StartAccept();
}