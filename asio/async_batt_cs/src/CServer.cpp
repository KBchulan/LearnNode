//
// Created by whx on 24-10-28.
//

#include "../include/CServer.hpp"

#include <iostream>

CServer::CServer(boost::asio::io_context &ioc, const unsigned short port) :
    _ioc(ioc), _acceptor(ioc, boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::any(), port)) {
    std::cout << "server start success, on port: " << port << std::endl;
    start_accept();
}

void CServer::clear_session(const std::string& uuid) {
    _sessions.erase(uuid);
}

void CServer::start_accept() {
    auto new_session = std::make_shared<CSession>(_ioc, this);
    _acceptor.async_accept(new_session->Socket(),
        [this, new_session]
            <typename T0>(T0 && PH1) {
                handle_accept(new_session, std::forward<T0>(PH1));
            });
}

void CServer::handle_accept(const std::shared_ptr<CSession>& new_session, const boost::system::error_code &error) {
    if(!error) {
        new_session->Start();
        _sessions.insert(std::make_pair(new_session->get_uuid(), new_session));
    }
    start_accept();
}
