#include <iostream>

#include "../include/CServer.hpp"
#include "../include/AsioIOServicePool.hpp"

int main()
{
    try
    {
        auto &pool = AsioIOServicePool::GetInstance();

        // 主线程所用ioc
        boost::asio::io_context ioc;
        boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);

        signals.async_wait([&ioc, &pool](const boost::system::error_code &error, int signal_number)
        { 
            ioc.stop(); 
            pool.Stop(); 
        });

        CServer server(ioc, 14789);
        ioc.run();
    }
    catch (const boost::system::system_error &error)
    {
        std::cerr << R"(Error value is: )" << error.code().value() << '\n';
        std::cerr << R"(Error message is: )" << error.what() << '\n';
    }
}