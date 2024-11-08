//
// Created by whx on 24-11-8.
//

#include <config.hpp>
#include <../include/MsgNode.hpp>

RecvNode::RecvNode(const long max_len, const long msg_id): MsgNode(max_len), _msg_id(msg_id) {}

SendNode::SendNode(const char *msg, const long max_len, const long msg_id) : MsgNode(max_len + HEAD_TOTAL_LENGTH), _msg_id(msg_id) {
    // 先发送ID
    const long msg_id_htonl = boost::asio::detail::socket_ops::host_to_network_long(_msg_id);
    memcpy(_data, &msg_id_htonl, HEAD_ID_LENGTH);

    // 发送长度
    long msg_len_htonl = boost::asio::detail::socket_ops::host_to_network_long(_total_len);
    memcpy(_data + HEAD_ID_LENGTH, &msg_len_htonl, HEAD_DATA_LENGTH);

    // 发送数据
    memcpy(_data + HEAD_TOTAL_LENGTH, msg, max_len);
}
