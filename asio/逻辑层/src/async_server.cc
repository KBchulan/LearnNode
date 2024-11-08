//
// Created by whx on 24-11-8.
//

#include <iostream>
#include <boost/asio.hpp>

int main(int argc, char **argv) {
    try {
        boost::asio::io_context ioc;

        ioc.run();
    } catch (const boost::system::system_error &error) {
        std::cout << R"(Ioc server failed!)" << '\n';
        std::cout << R"(The error code is: )" << error.code().value() << '\n';
        std::cout << R"(The error message is: )" << error.code().message() << '\n';
    }

    return 0;
}
