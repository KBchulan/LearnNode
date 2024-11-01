//
// Created by whx on 24-10-28.
//

#include "../include/CServer.hpp"
#include "../include/CSession.hpp"

void CSession::Start() {
    memset(_data, 0, max_length);

    _socket.async_read_some(boost::asio::buffer(_data, max_length),
        [this] <typename T0, typename T1>
            (T0 && PH1, T1 && PH2) {
                handle_read(std::forward<T0>(PH1), std::forward<T1>(PH2), shared_from_this());
            });
}

void CSession::Send(char *msg, int max_length) {
    std::lock_guard lock(_send_lock);
    const unsigned long send_size = _send_que.size();
    if(_send_que.size() > 50) {
        std::cout << "too much " << '\n';
        return;
    }

    _send_que.push(std::make_shared<MsgNode>(msg, max_length));
    if(send_size > 0) {
        return;
    }

    const auto& msgNode = _send_que.front();
    // ReSharper disable once CppRedundantQualifier
    boost::asio::async_write(_socket, boost::asio::buffer(msgNode->_data, msgNode->_total_len),
    [this]<typename T0, typename T1>
                (T0&& PH1, T1&&) {
            handle_write(std::forward<T0>(PH1), shared_from_this());
        });
}


void CSession::handle_read(const boost::system::error_code &error,
        std::size_t bytes_transferred, const std::shared_ptr<CSession>& _self_shared) {
    if(!error) {
        int copy_len = 0;
        while (bytes_transferred > 0) {
            if(!_head_parse) {
                // when length smaller than head
                if(bytes_transferred + _recv_head_node->_cur_len < HEAD_LENGTH) {
                    memcpy(_recv_head_node->_data + _recv_head_node->_cur_len, _data + copy_len, bytes_transferred);
                    _recv_head_node->_cur_len += static_cast<int>(bytes_transferred);

                    memset(_data, 0, max_length);
                    _socket.async_read_some(boost::asio::buffer(_data, max_length),
                    [this, _self_shared] <typename T0, typename T1>
                        (T0 && PH1, T1 && PH2){
                        handle_read(std::forward<T0>(PH1), std::forward<T1>(PH2), _self_shared);
                    });
                    return;
                }

                // 收集到的数据比头部多
                int head_remain = HEAD_LENGTH - _recv_head_node->_cur_len;
                memcpy(_recv_head_node->_data + _recv_head_node->_cur_len, _data + copy_len, head_remain);

                copy_len += head_remain;
                bytes_transferred -= head_remain;

                short data_len = 0;
                memcpy(&data_len, _recv_head_node->_data, HEAD_LENGTH);
                std::cout << "data length is: " << data_len << std::endl;

                if(data_len > max_length) {
                    std::cout << "invaild data length is: " << data_len << std::endl;
                    _server->clear_session(_uuid);
                    return;
                }
                _recv_msg_node = std::make_shared<MsgNode>(data_len);

                // 消息未收集全
                if(bytes_transferred < data_len) {
                    memcpy(_recv_msg_node->_data + _recv_msg_node->_cur_len, _data + copy_len, bytes_transferred);
                    _recv_msg_node->_cur_len += static_cast<int>(bytes_transferred);

                    memset(_data, 0, max_length);
                    _socket.async_read_some(boost::asio::buffer(_data, max_length),
                    [this, _self_shared] <typename T0, typename T1>
                            (T0 && PH1, T1 && PH2){
                        handle_read(std::forward<T0>(PH1), std::forward<T1>(PH2), _self_shared);
                    });

                    _head_parse = true;
                    return;
                }

                // 收集完数据
                memcpy(_recv_msg_node->_data + _recv_msg_node->_cur_len, _data + copy_len, data_len);

                _recv_msg_node->_cur_len += data_len;
                copy_len += data_len;
                bytes_transferred -= data_len;
                _recv_msg_node->_data[_recv_msg_node->_total_len] = '\0';

                std::cout << "receive data is: " << _recv_msg_node->_data << std::endl;
                Send(_recv_msg_node->_data, _recv_msg_node->_total_len);

                // 切包
                _head_parse = false;
                _recv_head_node->Clear();

                if(bytes_transferred <= 0) {
                    memset(_data, 0, max_length);
                    _socket.async_read_some(boost::asio::buffer(_data, max_length),
                    [this, _self_shared] <typename T0, typename T1>
                            (T0 && PH1, T1 && PH2){
                        handle_read(std::forward<T0>(PH1), std::forward<T1>(PH2), _self_shared);
                    });
                    return;
                }
                continue;
            }

            // 头部处理完了
            int remain_msg = _recv_msg_node->_total_len - _recv_msg_node->_cur_len;
            if(bytes_transferred < remain_msg) {
                memcpy(_recv_msg_node->_data + _recv_msg_node->_cur_len, _data + copy_len, bytes_transferred);

                _recv_msg_node->_cur_len += static_cast<int>(bytes_transferred);
                memset(_data, 0, max_length);
                _socket.async_read_some(boost::asio::buffer(_data, max_length),
                [this, _self_shared] <typename T0, typename T1>
                        (T0 && PH1, T1 && PH2){
                    handle_read(std::forward<T0>(PH1), std::forward<T1>(PH2), _self_shared);
                });
                return;
            }

            memcpy(_recv_msg_node->_data + _recv_msg_node->_cur_len, _data + copy_len, remain_msg);
            _recv_msg_node->_cur_len += remain_msg;
            bytes_transferred -= remain_msg;
            copy_len += remain_msg;
            _recv_msg_node->_data[_recv_msg_node->_total_len] = '\0';
            std::cout << "receive data is: " << _recv_msg_node->_data << std::endl;
            Send(_recv_msg_node->_data, _recv_msg_node->_total_len);

            // 切包
            _head_parse = false;
            _recv_head_node->Clear();

            if(bytes_transferred <= 0) {
                memset(_data, 0, max_length);
                _socket.async_read_some(boost::asio::buffer(_data, max_length),
                [this, _self_shared] <typename T0, typename T1>
                        (T0 && PH1, T1 && PH2){
                    handle_read(std::forward<T0>(PH1), std::forward<T1>(PH2), _self_shared);
                });
                return;
            }
        }
    }
    else {
        std::cout << R"(read error!)" << error.value() << "\n";
        _server->clear_session(_uuid);
    }
}

void CSession::handle_write(const boost::system::error_code &error, const std::shared_ptr<CSession>& _self_shared) {
    if(!error) {
        std::lock_guard lock(_send_lock);
        std::cout << "send data is: " << _send_que.front()->_data + HEAD_LENGTH << std::endl;
        _send_que.pop();
        if(!_send_que.empty()) {
            const auto &msgNode = _send_que.front();

            // ReSharper disable once CppRedundantQualifier
            boost::asio::async_write(_socket, boost::asio::buffer(msgNode->_data, msgNode->_total_len),
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