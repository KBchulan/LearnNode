//
// Created by whx on 24-10-26.
//

#ifndef SESSION_HPP
#define SESSION_HPP

#include <map>
#include <queue>
#include <memory>
#include <boost/asio.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

// 服务器会话类，echo类
// 此时在我们的ioc里只存在一个回调，但是若是读写同时存在回调，则会造成隐患

class Server;
class Session final :public std::enable_shared_from_this<Session>{
public:
    // 上下文
    explicit Session(boost::asio::io_context& ioc,
        Server* server):
        _socket(ioc), _data{}, _server(server) {
        const boost::uuids::uuid a_uuid = boost::uuids::random_generator()();
        _uuid = to_string(a_uuid);
    }

    [[nodiscard]] boost::asio::ip::tcp::socket& Socket() {
        return _socket;
    }

    // 监听客户端的读和写
    void Start();

    [[nodiscard]] std::string& get_uuid() {
        return _uuid;
    }

private:
    // 服务器读写的回调
    // 读用async_read_some
    void handle_read(const boost::system::error_code& error,
        size_t bytes_transferred, const std::shared_ptr<Session>& _self_shared);
    // 写用async_send
    void handle_write(const boost::system::error_code& error, const std::shared_ptr<Session>& _self_shared);

private:
    boost::asio::ip::tcp::socket _socket;
    enum {max_length = 1024};
    char _data[max_length];
    Server* _server;
    std::string _uuid;
};

class Server {
public:
    explicit Server(boost::asio::io_context& ioc, unsigned short port);

    void clear_session(const std::string& uuid);
private:
    // begin accept server
    void start_accept();

    // when the server receive some connection, the function will be played
    void handle_accept(const std::shared_ptr<Session>& new_session, const boost::system::error_code& error);

private:
    boost::asio::io_context& _ioc;
    boost::asio::ip::tcp::acceptor _acceptor;
    std::map<std::string, std::shared_ptr<Session>> _sessions;

};

#endif //SESSION_HPP
