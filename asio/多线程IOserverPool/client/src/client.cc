//
// Created by whx on 24-11-8.
//

#include <chrono>
#include <thread>
#include <iostream>
#include <boost/asio.hpp>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/reader.h>

#define MAX_LENGTH 2048
#define HEAD_ID_LENGTH 8
#define HEAD_DATA_LENGTH 8
#define HEAD_TOTAL_LENGTH 16

int main()
{
    std::vector<std::thread> threads;
    auto start = std::chrono::steady_clock::now();

    for (auto i = 0; i < 2; i++)
    {
        threads.emplace_back([]()
        {
            try
            {
                boost::asio::io_context ioc;
                boost::system::error_code error;
                const boost::asio::ip::tcp::endpoint remote_ep(boost::asio::ip::address::from_string(std::string("127.0.0.1")),
                                                       12569);
                boost::asio::ip::tcp::socket sock(ioc, remote_ep.protocol());

                sock.connect(remote_ep, error);
                if (error)
                {
                    std::cout << R"(Connect failed! Error code is: )" << error.value() << '\n'
                                   << R"(Error message is: )" << error.message() << '\n';
                    return 0;
                }

                int i = 0;
                while(i < 2)
                {
                    // 创建消息节点
                    Json::Value sendJsonData;
                    sendJsonData["id"] = 1001;
                    sendJsonData["body"] = "hello, logic!";

                    // Body
                    const std::string sendStringData = sendJsonData.toStyledString();

                    // Type, Length
                    constexpr long sendDataId = 1001;
                    const size_t sendDataLen = sendStringData.length();

                    const long sendDataIdNet = boost::asio::detail::socket_ops::host_to_network_long(sendDataId);
                    const long sendDataLenNet = boost::asio::detail::socket_ops::host_to_network_long(sendDataLen);

                    char sendData[MAX_LENGTH];
                    memcpy(sendData, &sendDataIdNet, HEAD_ID_LENGTH);
                    memcpy(sendData + HEAD_ID_LENGTH, &sendDataLenNet, HEAD_DATA_LENGTH);
                    memcpy(sendData + HEAD_TOTAL_LENGTH, sendStringData.c_str(), sendDataLen);

                    boost::asio::write(sock, boost::asio::buffer(sendData, sendDataLen + HEAD_TOTAL_LENGTH));
                    std::cout << R"(Begin to receive...)" << '\n';

                    // 开始接收数据
                    char receive_head[HEAD_TOTAL_LENGTH];
                    boost::asio::read(sock, boost::asio::buffer(receive_head, HEAD_TOTAL_LENGTH));

                    long recvDataId = 0;
                    long recvDataLen = 0;
                    char receive_body[MAX_LENGTH];

                    memcpy(&recvDataId, receive_head, HEAD_ID_LENGTH);
                    memcpy(&recvDataLen, receive_head + HEAD_ID_LENGTH, HEAD_DATA_LENGTH);

                    recvDataId = boost::asio::detail::socket_ops::network_to_host_long(recvDataId);
                    recvDataLen = boost::asio::detail::socket_ops::network_to_host_long(recvDataLen);

                    boost::asio::read(sock, boost::asio::buffer(receive_body, recvDataLen));

                    Json::Value receiveData;
                    Json::Reader reader;
                    reader.parse(std::string(receive_body, recvDataLen), receiveData);
                    std::cout << "Msg id is: " << receiveData["id"] << '\n'
                              << "Msg body is: " << receiveData["body"] << '\n';
                    i++;
                }
            }
            catch (const std::exception& e)
            {
                std::cerr << e.what() << std::endl;
            }
            return 0;
        });
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    for (auto &t:threads)
        t.join();

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time spent: " << duration.count() << std::endl;
    return 0;
}
