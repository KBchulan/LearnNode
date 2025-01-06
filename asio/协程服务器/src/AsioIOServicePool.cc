#include "../include/AsioIOServicePool.hpp"

#include <iostream>

AsioIOServicePool::AsioIOServicePool(std::size_t size) : _ioContexts(size), _works(size), _nextIOContext(0)
{
    // 构造_works
    for (std::size_t i = 0; i < size; i++)
    {
        _works[i] = std::unique_ptr<boost::asio::io_context::work>(
            new boost::asio::io_context::work(_ioContexts[i]));
    }

    // 遍历多个io_context，创建多个线程，每个线程启动对应io_context
    for (std::size_t i = 0; i < _ioContexts.size(); i++)
    {
        _threads.emplace_back([this, i]()
                              { _ioContexts[i].run(); });
    }
}

AsioIOServicePool::~AsioIOServicePool()
{
    std::cout << R"(AsioIOService Pool destruct!)" << std::endl;
}

AsioIOServicePool &AsioIOServicePool::GetInstance()
{
    static AsioIOServicePool instance;
    return instance;
}

boost::asio::io_context &AsioIOServicePool::GetIOContext()
{
    auto &context = _ioContexts[_nextIOContext++];
    _nextIOContext %= _ioContexts.size();
    return context;
}

void AsioIOServicePool::Stop()
{
    for (auto &work : _works)
    {
        work->get_io_context().stop();
        work.reset();
    }

    for (auto &thread : _threads)
    {
        thread.join();
    }
}
