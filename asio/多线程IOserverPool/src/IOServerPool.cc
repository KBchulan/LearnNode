//
// Created by whx on 24-11-13.
//

#include "../include/IOServerPool.hpp"

IOServerPool::~IOServerPool()
{
    std::cout << R"(IOServerPool has been destruceted!)" << std::endl;
}

boost::asio::io_context &IOServerPool::GetIOContext()
{
    auto &context = _ioContexts[_nextIOService++];

    if (_nextIOService == _ioContexts.size())
        _nextIOService = 0;

    return context;
}

void IOServerPool::Stop()
{
    for (auto &work : _works)
    {
        work.reset();
    }

    for (auto &t : _threads)
    {
        t.join();
    }
}

IOServerPool::IOServerPool(std::size_t size)
    : _ioContexts(size), _works(size), _nextIOService(0)
{
    // works
    for (std::size_t i = 0; i < size; i++)
    {
        _works[i] = std::unique_ptr<boost::asio::io_context::work>(new boost::asio::io_context::work(_ioContexts[i]));
    }

    // thread
    for (std::size_t i = 0; i < size; i++)
    {
        _threads.emplace_back([this, i]
                              { _ioContexts[i].run(); });
    }
}