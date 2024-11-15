#ifndef CSESSION_HPP
#define CSESSION_HPP

#include <memory>
#include <boost/asio.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

class CServer;
class CSession : public std::enable_shared_from_this<CSession>
{
public:
    explicit CSession(boost::asio::io_context &ioc, CServer *server);

    boost::asio::ip::tcp::socket &GetSocket();

    std::string &GetUuid();

    void Start();

private:
    CServer *_server;
    boost::asio::io_context& _ioc;
    boost::asio::ip::tcp::socket _socket;

private:
    std::string _uuid;


    
};

#endif // !CSESSION_HPP