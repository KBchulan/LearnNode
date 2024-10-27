//
// Created by whx on 24-10-26.
//

#ifndef SESSION_HPP
#define SESSION_HPP

#include <boost/asio.hpp>

// 服务器会话类,echo类
// 此时在我们的ioc里只存在一个回调，但是若是读写同时存在回调，则会造成隐患
class Session {
public:
    // 上下文
    explicit Session(boost::asio::io_context& ioc):
        _socket(ioc), _data{} {}

    boost::asio::ip::tcp::socket& Socket() {
        return _socket;
    }

    // 监听客户端的读和写
    void Start();

private:
    // 服务器读写的回调
    // 读用async_read_some
    void handle_read(const boost::system::error_code& error,
        size_t bytes_transferred);
    // 写用async_send
    void handle_write(const boost::system::error_code& error);

private:
    boost::asio::ip::tcp::socket _socket;
    enum {max_length = 1024};
    char _data[max_length];
};

class Server {
public:
    explicit Server(boost::asio::io_context& ioc, unsigned short port);

private:
    // begin accept server
    void start_accept();
    // when the server receive some connection, the function
    // will be played
    void handle_accept(Session* new_session, const boost::system::error_code& error);

private:
    boost::asio::io_context& _ioc;
    boost::asio::ip::tcp::acceptor _acceptor;

};

#endif //SESSION_HPP
