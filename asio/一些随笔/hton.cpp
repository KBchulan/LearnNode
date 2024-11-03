#include <boost/asio.hpp>
#include <iostream>

int main(){
    uint32_t host_long_value = 0x12345678;
    uint16_t host_short_value = 0x5678;

    uint32_t network_long_value = boost::asio::detail::socket_ops::host_to_network_long(host_long_value);
    uint16_t network_short_value = boost::asio::detail::socket_ops::host_to_network_short(host_short_value);

    std::cout << "host_long_value: " << std::hex << host_long_value << std::endl;
    std::cout << "network_long_value: " << std::hex << network_long_value << std::endl;

    std::cout << "host_short_value: " << std::hex << host_short_value << std::endl;
    std::cout << "network_short_value: " << std::hex << network_short_value << std::endl;

    return 0;
}