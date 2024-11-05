#include "../include/msg.pb.h"

#include <thread>
#include <memory>
#include <iostream>
#include <boost/asio.hpp>

constexpr int MAX_LENGTH = 1024 * 6;
constexpr int HEAD_LENGTH = 2;

int main()
{
    try
    {
        boost::asio::io_context ioc;
        boost::asio::ip::tcp::endpoint remote_ep(boost::asio::ip::address::from_string("127.0.0.1"), 12569);
        boost::asio::ip::tcp::socket sock(ioc, remote_ep.protocol());
        boost::system::error_code error = boost::asio::error::host_not_found;

        sock.connect(remote_ep, error);

        if (error)
        {
            std::cout << R"(connect failed!)" << '\n';
            std::cout << R"(error value is: )" << error.value() << '\n'
                      << R"(error message is: )" << error.message() << '\n';
            return 0;
        }

        // 构建节点
        std::shared_ptr<MsgData> sendMsgData = std::make_shared<MsgData>();
        sendMsgData->set_data("bnj");
        sendMsgData->set_id(1001);

        // protobuf序列化
        std::string sendData;
        sendMsgData->SerializeToString(&sendData);

        // 构建包头及包体
        char send[MAX_LENGTH];
        int sendLength = sendData.length();
        memcpy(send, &sendLength, HEAD_LENGTH);
        memcpy(send + HEAD_LENGTH, sendData.c_str(), sendLength);

        // 开始发送
        boost::asio::write(sock, boost::asio::buffer(send, sendLength + HEAD_LENGTH));

        // 开始接收
        std::cout << R"(begin to receive....)" << '\n';
        // head
        char receive_head[MAX_LENGTH];
        std::size_t receive_length = boost::asio::read(sock, boost::asio::buffer(receive_head, HEAD_LENGTH));
        short recv_length = 0;
        memcpy(&recv_length, receive_head, HEAD_LENGTH);
        // ntohs
        recv_length = boost::asio::detail::socket_ops::network_to_host_short(recv_length);
        // body
        char recv_msg[MAX_LENGTH] = {0};
        std::size_t msg_length = boost::asio::read(sock, boost::asio::buffer(recv_msg, MAX_LENGTH));

        // fan protobuf
        std::shared_ptr<MsgData> recvMsgData;
        recvMsgData->ParseFromArray(recv_msg, msg_length);
        std::cout << R"(receive id is: )" << recvMsgData->id() << R"(receive data is: )" << recvMsgData->data();
    }
    catch (const boost::system::system_error &err)
    {
        std::cout << err.what();
    }
}
