#include <iostream>
#include <boost/asio.hpp>

const int MAX_LENTH = 1024;

int main()
{
    try
    {
        boost::asio::io_context io_context(1);
        boost::asio::ip::tcp::endpoint remote_ep(boost::asio::ip::address(boost::asio::ip::address::from_string("127.0.0.1")), 14789);
        boost::system::error_code error = boost::asio::error::host_not_found;
        boost::asio::ip::tcp::socket socket(io_context);

        socket.connect(remote_ep, error);

        if (error)
        {
            std::cout << error.value() << " " << error.message() << std::endl;
            return 0;
        }

        std::cout << "Enter message: ";
        char request[MAX_LENTH];
        std::cin.getline(request, MAX_LENTH);

        size_t request_len = strlen(request);

        boost::asio::write(socket, boost::asio::buffer(request, request_len));

        char reply[MAX_LENTH];
        size_t reply_len = boost::asio::read(socket, boost::asio::buffer(reply, request_len));

        std::cout << "reply is: " << std::string(reply, reply_len) << '\n';
    }
    catch (const std::exception &e)
    {
        std::cerr << R"(Exception is: )" << e.what() << '\n';
    }
}