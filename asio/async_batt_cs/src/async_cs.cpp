//
// Created by whx on 24-10-28.
//

#include "../include/CServer.hpp"
#include "../include/CSession.hpp"

int main() {
    try {
        boost::asio::io_context ioc;
        CServer server(ioc, 12569);
        ioc.run();
    }catch(const boost::system::system_error& e) {
        std::cout << e.what() << std::endl;
    }
}