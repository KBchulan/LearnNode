#include <thread>
#include <memory>
#include <iostream>
#include <boost/asio.hpp>

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
    }
    catch (const boost::system::system_error &err)
    {
        std::cout << err.what();
    }
}
