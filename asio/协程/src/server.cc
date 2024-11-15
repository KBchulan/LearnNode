#include <iostream>
#include <coroutine>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>

int main()
{
    auto echo = [](boost::asio::ip::tcp::socket socket) -> boost::asio::awaitable<void>
    {
        try
        {
            char data[1024];
            while (true)
            {
                std::size_t len = co_await socket.async_read_some(boost::asio::buffer(data, 1024), boost::asio::use_awaitable);
                std::cout << std::string(data, len) << '\n';
                co_await boost::asio::async_write(socket, boost::asio::buffer(data, len), boost::asio::use_awaitable);
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << R"(Exception is: )" << e.what() << '\n';
        }
    };

    auto listener = [&echo]() -> boost::asio::awaitable<void>
    {
        // 异步查询当前协程的调度器，查不到就挂起
        auto executor = co_await boost::asio::this_coro::executor;

        boost::asio::ip::tcp::acceptor acceptor(executor, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 14789));
        while (true)
        {
            boost::asio::ip::tcp::socket socket = co_await acceptor.async_accept(boost::asio::use_awaitable);
            boost::asio::co_spawn(executor, echo(std::move(socket)), boost::asio::detached);
        }
    };

    try
    {
        boost::asio::io_context io_context(1);
        boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
        signals.async_wait([&](auto, auto)
                           { io_context.stop(); });

        // 分离协程执行监听
        boost::asio::co_spawn(io_context, listener(), boost::asio::detached);

        io_context.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << R"(Exception is: )" << e.what() << '\n';
    }
}