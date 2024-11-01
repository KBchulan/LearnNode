#include <thread>
#include <chrono>
#include <iostream>
#include <boost/asio.hpp>

int main()
{
    try
    {
        boost::asio::io_context ioc;
        boost::asio::ip::tcp::endpoint remote_ep(boost::asio::ip::address::from_string("127.0.0.1"), 12569);
        boost::asio::ip::tcp::socket sock(ioc, remote_ep.protocol());

        boost::system::error_code ec = boost::asio::error::host_not_found;
        sock.connect(remote_ep, ec);
        if (ec)
        {
            std::cout << "connect failed, code is " << ec.value() << " error msg is " << ec.message();
            return 0;
        }

        std::cout << R"(Please enter message: )";
        char request[2048];
        std::cin.getline(request, 2048);
        size_t request_length = strlen(request);

        char send_data[2048] = {0};
        memcpy(send_data, &request_length, 2);
        memcpy(send_data + 2, request, request_length);
        while (true)
        {
            boost::asio::write(sock, boost::asio::buffer(send_data, request_length + 2));

            char reply_head[2];
            size_t reply_length = boost::asio::read(sock, boost::asio::buffer(reply_head, 2));
            short msgLen = 0;
            memcpy(&msgLen, reply_head, 2);

            char msg[2024] = {0};
            size_t msgLength = boost::asio::read(sock, boost::asio::buffer(msg, msgLen));

            std::cout << "Reply is: ";
            std::cout.write(msg, msgLen) << std::endl;
            
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }
    catch (const boost::system::system_error &err)
    {
        std::cout << R"(error appear:)" << err.code().value() << std::endl;
    };
}