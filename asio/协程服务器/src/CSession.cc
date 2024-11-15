#include "../include/CSession.hpp"

CSession::CSession(boost::asio::io_context &ioc, CServer *server) : _ioc(ioc),
                                                                    _server(server), _socket(_ioc)
{
    boost::uuids::uuid a_uuid = boost::uuids::random_generator()();
    _uuid = boost::uuids::to_string(a_uuid);
}

boost::asio::ip::tcp::socket &CSession::GetSocket()
{
    return _socket;
}

std::string &CSession::GetUuid()
{
    return _uuid;
}

void CSession::Start()
{
    auto shared_this = shared_from_this();
}