//
// Created by whx on 24-10-26.
//

#include "../include/session.hpp"

Session::Session(const std::shared_ptr<boost::asio::ip::tcp::socket>& socket): _socket(socket), _send_pending(false), _recv_pending(false)
{

}

decltype(auto) Session::Connect(const boost::asio::ip::tcp::endpoint &ep) const {
    _socket->connect(ep);
}

void Session::WriteCallbackErr(const boost::system::error_code &ec, const std::size_t bytes_transferred,
    const std::shared_ptr<MsgNode>& msg_node) {
    if(bytes_transferred + msg_node->_cur_len < msg_node->_total_len) {
        _send_node->_cur_len += static_cast<int>(bytes_transferred);
        this->_socket->async_write_some(boost::asio::buffer(_send_node->_msg + _send_node->_cur_len, _send_node->_total_len - _send_node->_cur_len),
            [this](auto && PH1, auto && PH2) {
                WriteCallbackErr(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2), _send_node);
            });
    }
}

// epoll model, so it may have some error
void Session::WriteToSocketErr(const std::string &buf) {
    _send_node = std::make_shared<MsgNode>(buf.c_str(), buf.length());
    // if read success, the callback will be done
    this->_socket->async_write_some(boost::asio::buffer(_send_node->_msg, _send_node->_total_len),
        [this](auto && PH1, auto && PH2) {
            WriteCallbackErr(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2), _send_node);
        });
}

void Session::WriteCallback(const boost::system::error_code &ec, std::size_t bytes_transferred) {
    if (ec.value() != 0) {
        return;
    }
    const auto &send_data = _send_queue.front();
    send_data->_cur_len += static_cast<int>(bytes_transferred);
    if(send_data->_cur_len < send_data->_total_len) {
        this->_socket->async_write_some(boost::asio::buffer(send_data->_msg + send_data->_cur_len, send_data->_total_len - send_data->_cur_len),
        [this](auto && PH1, auto && PH2) {
            WriteCallback(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2));
        });
        return;
    }

    _send_queue.pop();
    if(_send_queue.empty()) {
        _send_pending = false;
    }
    else {
        const auto &_send_data = _send_queue.front();
        this->_socket->async_write_some(boost::asio::buffer(_send_data->_msg + _send_data->_cur_len, _send_data->_total_len - _send_data->_cur_len),
                [this](auto && PH1, auto && PH2) {
                    WriteCallback(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2));
                });
    }
}

void Session::WriteToSocket(const std::string &buf) {
    _send_queue.emplace(new MsgNode(buf.c_str(), static_cast<int>(buf.length())));
    if(_send_pending) {
        return;
    }
    this->_socket->async_write_some(boost::asio::buffer(buf),
    [this](auto && PH1, auto && PH2) {
        WriteCallback(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2));
    });
    _send_pending = true;
}

void Session::WriteAllCallback(const boost::system::error_code &ec, std::size_t bytes_transferred) {
    if (ec.value() != 0) {
        return;
    }
    _send_queue.pop();
    if(_send_queue.empty()) {
        _send_pending = false;
    }
}

void Session::WriteAllToSocket(const std::string &buf) {
    _send_queue.emplace(new MsgNode(buf.c_str(), static_cast<int>(buf.length())));
    // we can also use mutex to realize this func
    if (_send_pending) {
        return;
    }
    this->_socket->async_send(boost::asio::buffer(buf),
    [this](auto && PH1, auto && PH2) {
            WriteAllCallback(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2));
        });
    _send_pending = true;
}

void Session::ReadFromSocket() {
    if(_recv_pending)
        return;

    _recv_node = std::make_shared<MsgNode>(RECVSIZE);
    _socket->async_receive(boost::asio::buffer(_recv_node->_msg, _recv_node->_total_len),
    [this](auto && PH1, auto && PH2) {
        ReadCallBack(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2));
    });
    _recv_pending = true;
}

void Session::ReadCallBack(const boost::system::error_code &ec, std::size_t bytes_transferred) {
    _recv_node->_cur_len += static_cast<int>(bytes_transferred);
    _recv_node = nullptr;
    _recv_pending = false;
}

