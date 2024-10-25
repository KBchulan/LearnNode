//
// Created by whx on 24-10-25.
//

// 粘包，纯应答（全双工通信），进程存在线程上限（用ulimit修改治标不治本），切包

#include <set>
#include <thread>
#include <memory>
#include <iostream>
#include <boost/asio.hpp>

int main(int argc, char **argv) {
    constexpr size_t MAX_LENGTH = 1024;
    std::set<std::shared_ptr<std::thread>> thread_set;

    using socket_ptr = std::shared_ptr<boost::asio::ip::tcp::socket>;

    // 处理客户端的一个连接的读和写,在单独一个线程里
    auto session = [](const socket_ptr& sock) {
        try {
            while(true) {
                char reply[MAX_LENGTH] = {'\0'};
                boost::system::error_code error;

                // read
                // 此处不考虑粘包我们直接使用不阻塞的read_some
                const size_t length = sock->read_some(boost::asio::buffer(reply, MAX_LENGTH), error);

                if (error == boost::asio::error::eof) {
                    std::cout << R"(connection closed by peer)" << "\n";
                    break;
                }
                if(error)
                    throw boost::system::system_error(error);

                std::cout << R"(receive from:)" << sock->remote_endpoint().address().to_string() << std::endl;
                std::cout << R"(receive message is:)" << reply << std::endl;

                // send
                boost::asio::write(*sock, boost::asio::buffer(reply, length));

                std::this_thread::sleep_for(std::chrono::seconds(3));
            }
        }catch(const boost::system::system_error& e) {
            std::cout << R"(Error code: )" << e.code() << std::endl;
            std::cout << R"(Error message: )" << e.what() << std::endl;
        }
    };

     auto server = [&](boost::asio::io_context& ioc, const unsigned short port) {
        boost::asio::ip::tcp::acceptor accept(ioc, boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::any(), port));
        [[noreturn]] while (true) {
            socket_ptr sock(new boost::asio::ip::tcp::socket(ioc));
            accept.accept(*sock);
            auto thr = std::make_shared<std::thread>(session, sock);
            // 持久化，即扩大作用域
            thread_set.insert(thr);
            thr->detach();
        }
    };

    try {
        boost::asio::io_context ioc;
        const auto server_thread = std::make_shared<std::thread>(server, std::ref(ioc), 16686);
        server_thread->join();
    }catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
