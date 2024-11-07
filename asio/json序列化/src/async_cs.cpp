//
// Created by whx on 24-10-28.
//
// 我们采用网络字节序，就是成大端

#include "../include/CServer.hpp"
#include "../include/CSession.hpp"

int main() {
    try {
        boost::asio::io_context ioc;
        CServer server(ioc, 12569);
        // 跑消息循环，类似于一个有间歇的while循环，当有哪个事件就绪了，就进行派发
        ioc.run();
    }catch(const boost::system::system_error& e) {
        std::cout << e.what() << std::endl;
    }
}