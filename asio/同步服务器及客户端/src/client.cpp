//
// Created by whx on 24-10-25.
//

#include <thread>
#include <chrono>
#include <iostream>
#include <boost/asio.hpp>

int main(int argc, char **argv)
{
    try
    {
        constexpr size_t MAX_LENGTH = 1024;
        // 上下文服务
        boost::asio::io_context ioc;
        // 端口
        constexpr unsigned short port = 12569;
        // 远程服务器端点
        const boost::asio::ip::tcp::endpoint
            remote_ep(boost::asio::ip::address::from_string(std::string("127.0.0.1")), port);
        boost::system::error_code error = boost::asio::error::host_not_found;
        boost::asio::ip::tcp::socket sock(ioc, remote_ep.protocol());

        sock.connect(remote_ep, error); // NOLINT(*-unused-return-value)
        if (error)
        {
            std::cout << R"(connect failed,code is: )" << error.value() << std::endl;
            std::cout << R"(Error message: )" << error.message() << std::endl;
            return 0;
        }

        // 开始读写
        std::cout << R"(Please enter message: )";
        char request[MAX_LENGTH];
        std::cin.getline(request, MAX_LENGTH);
        const size_t request_length = strlen(request);
        boost::asio::write(sock, boost::asio::buffer(request, request_length));

        while (true)
        {
            char reply[MAX_LENGTH];
            const size_t reply_length = boost::asio::read(sock, boost::asio::buffer(reply, request_length));
            std::cout << R"(Reply is: )";
            std::cout.write(reply, static_cast<long>(reply_length));
            std::cout << "\n";

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    catch (const boost::system::system_error &e)
    {
        std::cout << R"(Error code: )" << e.code() << std::endl;
        std::cout << R"(Error message: )" << e.what() << std::endl;
        return e.code().value();
    }
}
