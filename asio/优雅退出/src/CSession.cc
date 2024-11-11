//
// Created by whx on 24-11-9.
//

#include "../include/CSession.hpp"

#include "CServer.hpp"
#include <LogicSystem.hpp>
#include <jsoncpp/json/value.h>

CSession::CSession(boost::asio::io_context &ioc, CServer *server) : _b_close(false), _b_head_parse(false), _data{},
                                                                    _server(server), _socket(ioc) {
    const boost::uuids::uuid a_uuid = boost::uuids::random_generator()();
    _uuid = to_string(a_uuid);
    _recv_head_node = std::make_shared<MsgNode>(HEAD_TOTAL_LENGTH);
}

CSession::~CSession() {
    std::cout << "CSession destruct" << std::endl;
}

void CSession::Start() {
    memset(_data, 0, MAX_LENGTH);
    _socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH),
                            [this]<typename T0, typename T1>(T0 &&PH1, T1 &&PH2) {
                                HandleRead(std::forward<T0>(PH1), std::forward<T1>(PH2), SharedSelf());
                            });
}

void CSession::Close() {
    _socket.close();
    _b_close = true;
}

void CSession::Send(const std::string &msg, long msg_id) {
    std::lock_guard locker(_send_lock);
    const int send_que_size = static_cast<int>(_send_queue.size());

    if (send_que_size > MAX_SENDQUE) {
        std::cout << "session: " << _uuid << " send que fulled, size is " << MAX_SENDQUE << '\n';
        return;
    }

    _send_queue.push(std::make_shared<SendNode>(msg.c_str(), msg.length(), msg_id));
    if (send_que_size > 0)
        return;

    auto &msgNode = _send_queue.front();
    boost::asio::async_write(_socket, boost::asio::buffer(msgNode->_data, msgNode->_total_len),
                             [this]<typename T0, typename T1>
                     (T0 &&PH1, T1 &&) {
                                 HandleWrite(std::forward<T0>(PH1), SharedSelf());
                             });
}

void CSession::Send(char *msg, long max_length, long msg_id) {
    std::lock_guard locker(_send_lock);
    const int send_que_size = static_cast<int>(_send_queue.size());

    if (send_que_size > MAX_SENDQUE) {
        std::cout << "session: " << _uuid << " send que fulled, size is " << MAX_SENDQUE << '\n';
        return;
    }

    _send_queue.push(std::make_shared<SendNode>(msg, max_length, msg_id));
    if (send_que_size > 0)
        return;

    const auto &msgNode = _send_queue.front();
    boost::asio::async_write(_socket, boost::asio::buffer(msgNode->_data, msgNode->_total_len),
                             [this]<typename T0, typename T1>
                     (T0 &&PH1, T1 &&) {
                                 HandleWrite(std::forward<T0>(PH1), SharedSelf());
                             });
}

boost::asio::ip::tcp::socket &CSession::GetSocket() {
    return _socket;
}

std::string &CSession::GetUuid() {
    return _uuid;
}

std::shared_ptr<CSession> CSession::SharedSelf() {
    return shared_from_this();
}

void CSession::HandleRead(const boost::system::error_code &ec, size_t bytes_transferred,
                          const std::shared_ptr<CSession>& shared_self) {
    try {
        if (!ec) {
            // 已经移动的字符串
            int copy_len = 0;
            while (bytes_transferred > 0) {
                if (!_b_head_parse) {
                    // 收到的数据不足头部大小
                    if (bytes_transferred + _recv_head_node->_cur_len < HEAD_TOTAL_LENGTH) {
                        memcpy(_recv_head_node->_data + _recv_head_node->_cur_len, _data + copy_len, bytes_transferred);
                        _recv_head_node->_cur_len += static_cast<long>(bytes_transferred);

                        memset(_data, 0, MAX_LENGTH);
                        _socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH),
                                                [this, shared_self]<typename T0, typename T1>(T0 &&PH1, T1 &&PH2) {
                                                    HandleRead(std::forward<T0>(PH1), std::forward<T1>(PH2),
                                                               shared_self);
                                                });
                        return;
                    }
                    // 收到的数据比头部（TL）长
                    // 取出头部剩下的数据
                    const int head_remain = HEAD_TOTAL_LENGTH - _recv_head_node->_cur_len;
                    memcpy(_recv_head_node->_data + _recv_head_node->_cur_len, _data + copy_len, head_remain);

                    // 更新已经处理的data长度和未处理的长度
                    copy_len += head_remain;
                    bytes_transferred -= head_remain;

                    // 获取ID
                    long msg_id = 0;
                    memcpy(&msg_id, _recv_head_node->_data, HEAD_ID_LENGTH);
                    msg_id = boost::asio::detail::socket_ops::network_to_host_long(msg_id);

                    // 输出ID
                    std::cout << "msg_id is: " << msg_id << '\n';
                    if(msg_id > MAX_LENGTH) {
                        std::cout << "Invaile id is: " << msg_id << '\n';
                        _server->ClearSession(_uuid);
                        return;
                    }

                    // 获取长度
                    long msg_len = 0;
                    memcpy(&msg_len, _recv_head_node->_data + HEAD_ID_LENGTH, HEAD_DATA_LENGTH);
                    msg_len = boost::asio::detail::socket_ops::network_to_host_long(msg_len);

                    // 输出长度
                    std::cout << "msg_len is: " << msg_len << '\n';
                    if(msg_len > MAX_LENGTH) {
                        std::cout << "Invaile length is: " << msg_len << '\n';
                        _server->ClearSession(_uuid);
                        return;
                    }

                    _recv_msg_node = std::make_shared<RecvNode>(msg_len, msg_id);

                    // 消息长度少于应有的头部取出长度，先拿出一部分
                    if(bytes_transferred < msg_len) {
                        memcpy(_recv_msg_node->_data + _recv_msg_node->_cur_len, _data + copy_len, bytes_transferred);
                        _recv_msg_node->_cur_len += static_cast<long>(bytes_transferred);

                        memset(_data, 0, MAX_LENGTH);
                        _socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH),
                                                [this, shared_self]<typename T0, typename T1>(T0 &&PH1, T1 &&PH2) {
                                                    HandleRead(std::forward<T0>(PH1), std::forward<T1>(PH2),
                                                               shared_self);
                                                });
                        _b_head_parse = true;
                        return;
                    }

                    // 收集到的数据比头部给出长度长
                    memcpy(_recv_msg_node->_data + _recv_msg_node->_cur_len, _data + copy_len, msg_len);
                    _recv_msg_node->_cur_len += msg_len;
                    copy_len += static_cast<int>(msg_len);
                    bytes_transferred -= msg_len;
                    _recv_msg_node->_data[_recv_msg_node->_total_len] = '\0';

                    LogicSystem::GetInstance()->PostMsgToQueue(std::make_shared<LogicNode>(shared_from_this(), _recv_msg_node));

                    // 继续轮询未查找数据
                    _b_head_parse = false;
                    _recv_head_node->Clear();
                    if(bytes_transferred <= 0) {
                        memset(_data, 0, MAX_LENGTH);
                        _socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH),
                                                [this, shared_self]<typename T0, typename T1>(T0 &&PH1, T1 &&PH2) {
                                                    HandleRead(std::forward<T0>(PH1), std::forward<T1>(PH2),
                                                               shared_self);
                                                });
                        return;
                    }
                    continue;
                }


                // 此时已经处理完头部，继续处理上次未接受完的数据
                // 接受数据少于未处理的
                const int remain_msg = static_cast<int>(_recv_msg_node->_total_len - _recv_msg_node->_cur_len);
                if(bytes_transferred < remain_msg) {
                    memcpy(_recv_msg_node->_data + _recv_msg_node->_cur_len, _data + copy_len, bytes_transferred);
                    _recv_msg_node->_cur_len += static_cast<long>(bytes_transferred);

                    memset(_data, 0, MAX_LENGTH);
                    _socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH),
                                            [this, shared_self]<typename T0, typename T1>(T0 &&PH1, T1 &&PH2) {
                                                HandleRead(std::forward<T0>(PH1), std::forward<T1>(PH2),
                                                           shared_self);
                                            });
                    return;
                }

                // 收集完剩余数据
                memcpy(_recv_msg_node->_data + _recv_msg_node->_cur_len, _data + copy_len, remain_msg);
                _recv_msg_node->_cur_len += remain_msg;
                bytes_transferred -= remain_msg;
                copy_len += remain_msg;
                _recv_msg_node->_data[_recv_msg_node->_total_len] = '\0';

                LogicSystem::GetInstance()->PostMsgToQueue(std::make_shared<LogicNode>(shared_from_this(), _recv_msg_node));

                // 继续轮询
                _b_head_parse = false;
                _recv_head_node->Clear();
                if(bytes_transferred <= 0) {
                    memset(_data, 0, MAX_LENGTH);
                    _socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH),
                                            [this, shared_self]<typename T0, typename T1>(T0 &&PH1, T1 &&PH2) {
                                                HandleRead(std::forward<T0>(PH1), std::forward<T1>(PH2),
                                                           shared_self);
                                            });
                    return;
                }
            }
        }

        else {
            std::cout << "handle read failed, error is " << ec.message() << '\n';
            Close();
            _server->ClearSession(_uuid);
        }
    } catch (const boost::system::system_error &e) {
        std::cout << "Exception code is " << e.what() << std::endl;
    }
}

void CSession::HandleWrite(const boost::system::error_code &ec, const std::shared_ptr<CSession> &shared_self) {
    try {
        if (!ec) {
            std::lock_guard lock(_send_lock);
            _send_queue.pop();
            if (!_send_queue.empty()) {
                const auto &msgNode = _send_queue.front();
                boost::asio::async_write(_socket, boost::asio::buffer(msgNode->_data, msgNode->_total_len),
                                         [this, shared_self]<typename T0, typename T1>
                                 (T0 &&PH1, T1 &&) {
                                             HandleWrite(std::forward<T0>(PH1), shared_self);
                                         });
            }
        } else {
            std::cout << "handle write failed, error is " << ec.message() << '\n';
            Close();
            _server->ClearSession(_uuid);
        }
    } catch (std::exception &e) {
        std::cerr << "Exception code : " << e.what() << '\n';
    }
}
