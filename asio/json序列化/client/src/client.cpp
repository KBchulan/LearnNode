#include <thread>
#include <memory>
#include <iostream>
#include <boost/asio.hpp>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/writer.h>

constexpr int MAX_LENGTH = 1024 * 2;
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
        
        // define json message
        Json::Value sendJsonData;
        sendJsonData["id"] = 1001;
        sendJsonData["message"] = "hello, jsonServer!";

        // the fold header and body
        std::string sendDataStr = sendJsonData.toStyledString();
        short sendDataLen = boost::asio::detail::socket_ops::host_to_network_short(sendDataStr.length());

        char sendData[MAX_LENGTH];
        memcpy(sendData, &sendDataLen, HEAD_LENGTH);
        memcpy(sendData + HEAD_LENGTH, sendDataStr.c_str(), sendDataStr.length());

        boost::asio::write(sock, boost::asio::buffer(sendData, sendDataStr.length() + HEAD_LENGTH));
        std::cout << R"(begin to receive...)" << '\n';

        // header
        char recvHead[HEAD_LENGTH];
        boost::asio::read(sock, boost::asio::buffer(recvHead, HEAD_LENGTH));
        short recvLengthHost;
        memcpy(&recvLengthHost, recvHead, HEAD_LENGTH);
        recvLengthHost = boost::asio::detail::socket_ops::network_to_host_short(recvLengthHost);

        // body
        char recvBody[MAX_LENGTH];
        boost::asio::read(sock, boost::asio::buffer(recvBody, recvLengthHost));

        Json::Value recvJsonData;
        Json::Reader reader;
        reader.parse(std::string(recvBody, recvLengthHost), recvJsonData);

        std::cout << "id is: " << recvJsonData["id"] << '\n' << "message is: " << recvJsonData["message"] << '\n';
        getchar();
    }
    catch (const boost::system::system_error &err)
    {
        std::cout << err.what();
    }
}
