//
// Created by whx on 24-10-28.
//

#include "../include/CServer.hpp"
#include "../include/CSession.hpp"

void CSession::Start() {
    memset(_data, 0, max_length);

    _socket.async_read_some(boost::asio::buffer(_data, max_length),
        [this] <typename T0, typename T1>
            (T0 && PH1, T1 && PH2){
                handle_read(std::forward<T0>(PH1), std::forward<T1>(PH2), shared_from_this());
            });
}

void CSession::Send(char *msg, int max_length) {
    // 为true时表示上次数据还未发送完
    bool pending = false;

    std::lock_guard lock(_send_lock);
    if(!_send_que.empty())
        pending = true;

    _send_que.push(std::make_shared<MsgNode>(msg, max_length));
    if(pending)
        return;

    // ReSharper disable once CppRedundantQualifier
    boost::asio::async_write(_socket, boost::asio::buffer(msg, max_length),
    [this]<typename T0, typename T1>
                (T0&& PH1, T1&&) {
            handle_write(std::forward<T0>(PH1), shared_from_this());
        });
}


void CSession::handle_read(const boost::system::error_code &error,
                          const std::size_t bytes_transferred, const std::shared_ptr<CSession>& _self_shared) {
    if(!error) {
        std::cout << R"(server receive data is : )" << _data << "\n";
        Send(_data, static_cast<int>(bytes_transferred));
        memset(_data, 0, max_length);

        _socket.async_read_some(boost::asio::buffer(_data, max_length),
        [this, _self_shared]<typename T0, typename T1>
                 (T0&& PH1, T1&& PH2) {
             handle_read(std::forward<T0>(PH1), std::forward<T1>(PH2), _self_shared);
         });

    }
    else {
        std::cout << R"(read error!)" << error.value() << "\n";
        _server->clear_session(_uuid);
    }
}

void CSession::handle_write(const boost::system::error_code &error, const std::shared_ptr<CSession>& _self_shared) {
    if(!error) {
        std::lock_guard lock(_send_lock);
        _send_que.pop();
        if(!_send_que.empty()) {
            const auto &msgNode = _send_que.front();

            // ReSharper disable once CppRedundantQualifier
            boost::asio::async_write(_socket, boost::asio::buffer(msgNode->_data, msgNode->_max_len),
        [this, _self_shared]<typename T0, typename T1>
                (T0&& PH1, T1&&) {
                handle_write(std::forward<T0>(PH1), _self_shared);
            });
        }
    }
    else {
        std::cout << R"(write error!)" << error.value() << "\n";
        _server->clear_session(_uuid);
    }
}