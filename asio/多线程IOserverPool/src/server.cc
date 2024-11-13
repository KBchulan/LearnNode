//
// Created by whx on 24-11-8.
//
// asio to exit this server

#include "../include/CServer.hpp"

#include <mutex>
#include <thread>
#include <condition_variable>

// 主循环是监听的

int main(int argc, char **argv)
{
    try
    {
        boost::asio::io_context ioc;
        
        // events
        boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait([&ioc](auto, auto)
                           { ioc.stop(); });

        std::shared_ptr<CServer> server = std::make_shared<CServer>(ioc, 12569);
        ioc.run();
    }
    catch (const boost::system::system_error &error)
    {
        std::cout << R"(Ioc server failed!)" << '\n';
        std::cout << R"(The error code is: )" << error.code().value() << '\n';
        std::cout << R"(The error message is: )" << error.code().message() << '\n';
    }

    std::cerr << "Server stopped gracefully" << std::endl;

    return 0;
}
