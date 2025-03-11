#include <iostream>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/io_context.hpp>

int main()
{
    auto echo = [](boost::asio::ip::tcp::socket socket) -> boost::asio::awaitable<void>
    {
        try
        {
            char data[1024];
            std::size_t len = co_await socket.async_read_some(boost::asio::buffer(data, 1024), boost::asio::use_awaitable);
            std::cout << std::string(data, len) << std::endl;
            co_await boost::asio::async_write(socket, boost::asio::buffer(data, len), boost::asio::use_awaitable);
        }
        catch (const boost::system::system_error &error)
        {
            std::cerr << error.code() << '\n';
            std::cerr << error.what() << '\n';
        }
    };

    auto listener = [&echo]() -> boost::asio::awaitable<void>
    {
        auto executor = co_await boost::asio::this_coro::executor;
        boost::asio::ip::tcp::acceptor acceptor(executor, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 14789));
        while (true)
        {
            auto socket = co_await acceptor.async_accept(boost::asio::use_awaitable);
            boost::asio::co_spawn(executor, echo(std::move(socket)), boost::asio::detached);
        }
    };

    try
    {
        boost::asio::io_context ioc;
        boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);

        signals.async_wait([&](auto, auto)
                           { ioc.stop(); });

        boost::asio::co_spawn(ioc, listener(), boost::asio::detached);
        ioc.run();
    }
    catch (const boost::system::system_error &error)
    {
        std::cerr << error.code() << '\n';
        std::cerr << error.what() << '\n';
    }
}