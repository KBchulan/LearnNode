//
// Created by whx on 24-11-13.
//

#ifndef IOSERVERPOOL_HPP
#define IOSERVERPOOL_HPP

#include <memory>
#include <vector>
#include <Singleton.hpp>
#include <boost/asio.hpp>

class IOServerPool final : public Singleton<IOServerPool>
{
    friend class Singleton<IOServerPool>;

public:
    ~IOServerPool();

    // 使用轮询返回一个io_context
    boost::asio::io_context &GetIOContext();

    void Stop();

private:
    IOServerPool(std::size_t size = std::thread::hardware_concurrency());

    IOServerPool(const IOServerPool &) = delete;
    IOServerPool &operator=(const IOServerPool &) = delete;

private:
    std::size_t _nextIOService;
    std::vector<std::thread> _threads;
    std::vector<boost::asio::io_context> _ioContexts;
    std::vector<std::unique_ptr<boost::asio::io_context::work>> _works;
};
#endif // !IOSERVERPOOL_HPP