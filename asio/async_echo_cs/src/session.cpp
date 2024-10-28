//
// Created by whx on 24-10-26.
//

#include "../include/session.hpp"

#include <iostream>

void Session::Start() {
    memset(_data, 0, max_length);

    _socket.async_read_some(boost::asio::buffer(_data, max_length),
        [this] <typename T0, typename T1>
            (T0 && PH1, T1 && PH2){
                handle_read(std::forward<T0>(PH1), std::forward<T1>(PH2));
            });
}

void Session::handle_read(const boost::system::error_code &error,
                          const std::size_t bytes_transferred) {
    if(!error) {
        std::cout << R"(server receive data is :)" << _data << "\n";

        // ReSharper disable once CppRedundantQualifier
        boost::asio::async_write(_socket, boost::asio::buffer(_data, bytes_transferred),
            [this]<typename T0, typename T1>
                    (T0&& PH1, T1&&) {
                handle_write(std::forward<T0>(PH1));
            });

        memset(_data, 0, max_length);

        _socket.async_read_some(boost::asio::buffer(_data, max_length),
        [this]<typename T0, typename T1>
            (T0 && PH1, T1 && PH2){
                handle_read(std::forward<T0>(PH1), std::forward<T1>(PH2));
            });
    }
    else {
        std::cout << R"(read error!)" << error.value() << "\n";
        delete this;
    }
}

void Session::handle_write(const boost::system::error_code &error) {
    if(!error) {
        memset(_data, 0, max_length);

        _socket.async_read_some(boost::asio::buffer(_data, max_length),
        [this]<typename T0, typename T1>
            (T0 && PH1, T1 && PH2){
                handle_read(std::forward<T0>(PH1), std::forward<T1>(PH2));
            });
    }
    else {
        std::cout << R"(write error!)" << error.value() << "\n";
        delete this;
    }
}

Server::Server(boost::asio::io_context &ioc, const unsigned short port) :
    _ioc(ioc), _acceptor(ioc, boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::any(), port)) {
    std::cout << "server start success, on port: " << port << std::endl;
    start_accept();
}

void Server::start_accept() {
    auto* new_session = new Session(_ioc);
    _acceptor.async_accept(new_session->Socket(),
        [this, new_session]
            <typename T0>(T0 && PH1) {
                handle_accept(new_session, std::forward<T0>(PH1));
            });
}

void Server::handle_accept(Session *new_session, const boost::system::error_code &error) {
    if(!error)
        new_session->Start();
    else
        delete new_session;
    start_accept();
}
