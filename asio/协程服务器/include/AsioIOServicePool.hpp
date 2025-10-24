#ifndef ASIOIOSERVICEPOOL_HPP
#define ASIOIOSERVICEPOOL_HPP

#include <atomic>
#include <vector>
#include <memory>
#include <boost/asio.hpp>

class AsioIOServicePool
{
public:
    ~AsioIOServicePool();

    AsioIOServicePool(const AsioIOServicePool &) = delete;
    AsioIOServicePool &operator=(const AsioIOServicePool &) = delete;
    
    // 停止服务池
    void Stop();
    
    // 轮询查询io_context
    boost::asio::io_context &GetIOContext();

    // 服务池实例对象
    static AsioIOServicePool &GetInstance();

private:
    std::size_t _nextIOContext;
    std::vector<std::thread> _threads;
    std::vector<boost::asio::io_context> _ioContexts;
    std::vector<std::unique_ptr<boost::asio::io_context::work>> _works;

    AsioIOServicePool(std::size_t size = std::thread::hardware_concurrency());
};

#endif // !ASIOIOSERVICEPOOL_HPP