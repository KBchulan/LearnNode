#include "msg.pb.h"

#include <thread>
#include <iostream>
#include <boost/asio.hpp>

constexpr int MAX_LENGTH = 1024 * 2;
constexpr int HEAD_LENGTH = 2;

int main()
{
    try{
        boost::asio::io_context ioc;
        boost::asio::ip::tcp::endpoint remote_ep(boost::asio::ip::address::from_string(std::string("127.0.0.1")), 12569);
        boost::asio::ip::tcp::socket sock(ioc, remote_ep.protocol());

    }catch (const boost::system::system_error& err){
        std::cout << err.what();
    }
}
