//
// Created by whx on 24-10-27.
//

#include "../include/session.hpp"

#include <iostream>
#include <boost/asio.hpp>

int main() {
    try {
        boost::asio::io_context ioc;
        Server server(ioc, 12569);
        ioc.run();
    }catch(const boost::system::system_error& e) {
        std::cout << e.what() << std::endl;
    }
}
