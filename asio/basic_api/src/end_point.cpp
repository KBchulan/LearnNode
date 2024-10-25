//
// Created by whx on 24-10-24.
//

#include "../include/end_point.hpp"

#include <vector>
#include <iostream>
#include <boost/asio.hpp>

int client_end_point() {
    // 客户端：根据对端的ip和端口构造
    const auto raw_ip_address = std::string("100.67.81.233");
    constexpr unsigned short port_num = 8080;
    boost::system::error_code ec;

    // 这个error_code不是很有必要
    const boost::asio::ip::address ip_address = boost::asio::ip::address::from_string(raw_ip_address, ec);
    if(ec.value() != 0) {
        std::cout << "Failed to parse the IP address." << std::endl;
        std::cout << "Error code:" << ec.value() << std::endl;
        return ec.value();
    }

    // 客户端通过ep进行连接
    [[maybe_unused]] boost::asio::ip::tcp::endpoint ep(ip_address, port_num);
    return 0;
}

int server_end_point() {
    constexpr unsigned short port_num = 8080;

    // 任何地址都可以通信
    const boost::asio::ip::address ip_address = boost::asio::ip::address_v6::any();

    [[maybe_unused]] boost::asio::ip::tcp::endpoint ep(ip_address, port_num);
    return 0;
}

int create_tcp_socket() {
    // socket用来通信的参数：上下文
    boost::asio::io_context ioc;
    // 创建协议，打开的时候需要用到
    [[maybe_unused]] const boost::asio::ip::tcp protocol = boost::asio::ip::tcp::v4();
    // socket
    boost::asio::ip::tcp::socket sock(ioc);
    return 0;
}

int create_accept_socket() {
    // 与tcp的socket一样
    boost::asio::io_context ioc;
    [[maybe_unused]] const boost::asio::ip::tcp protocol = boost::asio::ip::tcp::v4();
    boost::asio::ip::tcp::acceptor
        acceptor(ioc, boost::asio::ip::tcp::endpoint(protocol, 8080));
    return 0;
}

int bind_accept_socket() {
    constexpr unsigned short port = 3332;
    const boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::any(), port);
    boost::asio::io_context ioc;
    boost::asio::ip::tcp::acceptor
        acceptor(ioc, ep);
    return 0;
}

int connect_to_endpoint() {
    const auto raw_ip_address = std::string("127.0.0.1");
    try {
        constexpr unsigned short port = 8080;
        const boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(raw_ip_address), port);
        boost::asio::io_context ioc;
        boost::asio::ip::tcp::socket sock(ioc, ep.protocol());
        sock.connect(ep);
    }catch (const boost::system::system_error& e) {
        return e.code().value();
    }
    return 0;
}

int dns_connect_to_endpoint() {
    // 可以通过脚本转成ip
    const auto host = std::string("github.com/KBchulan?tab=repositories");
    const auto port = std::string("3333");
    boost::asio::io_context ioc;

    // 查询器, 客户端连接服务器，做域名解析，最后的flag选用numeric_service
    const boost::asio::ip::tcp::resolver::query resolver_query(host, port, boost::asio::ip::tcp::resolver::query::numeric_service);
    // 解析
    boost::asio::ip::tcp::resolver resolver(ioc);

    try {
        const boost::asio::ip::tcp::resolver::iterator it = resolver.resolve(resolver_query);
        boost::asio::ip::tcp::socket sock(ioc);
        // boost::asio::connect(sock, it);

    }catch(const boost::system::system_error& e){
        return e.code().value();
    }
    return 0;
}

int accept_new_connection() {
    constexpr unsigned short port = 3333;
    const boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::any(), port);

    try {
        constexpr int BACKLOG_SIZE = 30;
        boost::asio::io_context ioc;
        boost::asio::ip::tcp::acceptor
            acceptor(ioc, ep.protocol());

        acceptor.bind(ep);
        acceptor.listen(BACKLOG_SIZE);
        boost::asio::ip::tcp::socket sock(ioc, ep.protocol());
        acceptor.accept(sock);

    }catch(const boost::system::system_error& e){
        return e.code().value();
    }
    return 0;
}

void use_const_buffer() {
    const auto buf(std::string("Hello, buffer!"));
    // const_buffer_1是对应的适配器
    const boost::asio::const_buffer asio_buf(buf.c_str(), buf.size());
    std::vector<boost::asio::const_buffer> buffers_sequence;
    buffers_sequence.push_back(asio_buf);
    // send(buffers_sequence); 伪代码，并不对，思路基本是这样
    // 因为send传入的是const_buffer_sequence类型的，构造一个即可，每个是const_buffer

}

void use_buffer_str() {
    // 跟上面那个函数一个效果
    [[maybe_unused]] boost::asio::const_buffers_1 output_buf = boost::asio::buffer("hello,buffer");
}

void use_buffer_array() {
    constexpr size_t BUFFER_SIZE = 20;
    const std::unique_ptr<char []> buf(new char [BUFFER_SIZE]);
    [[maybe_unused]] auto input_bur = boost::asio::buffer(buf.get(), BUFFER_SIZE);
}

// 下面写一下同步读写的功能
void write_to_socket(boost::asio::ip::tcp::socket& sock) {
    const auto buf(std::string("hello, buffer"));
    size_t total_bytes_written = 0;
    // write_some, 返回每次写入的字节数
    while(total_bytes_written != buf.size()) {
        total_bytes_written += sock.write_some(boost::asio::buffer(buf.c_str() +
            total_bytes_written, buf.length() - total_bytes_written));
    }
}

// use write_to_socket
int send_data_by_write_some() {
    const auto raw_ip_address(std::string("100.65.140.179"));

    try {
        constexpr unsigned short port = 8089;
        const boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(raw_ip_address), port);
        boost::asio::io_context ioc;
        boost::asio::ip::tcp::socket sock(ioc, ep.protocol());

        sock.connect(ep);
        write_to_socket(sock);
    }catch(const boost::system::system_error& e) {
        std::cout << "Error code:" << e.code() << std::endl;
        std::cout << "Error message:" << e.what() << std::endl;
        return e.code().value();
    }
    return 0;
}

// use send
int send_data_by_send() {
    const auto raw_ip_address(std::string("100.65.140.179"));

    try {
        constexpr unsigned short port = 8089;
        const boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(raw_ip_address), port);
        boost::asio::io_context ioc;
        boost::asio::ip::tcp::socket sock(ioc, ep.protocol());

        sock.connect(ep);
        const auto buf(std::string("hello, buffer"));
        // user ---> tcp
        [[maybe_unused]] unsigned long send_length = sock.send(boost::asio::buffer(buf.c_str(), buf.size()));
        // send_length<0 tcp error    =0:ep error
    }catch(const boost::system::system_error& e) {
        std::cout << "Error code:" << e.code() << std::endl;
        std::cout << "Error message:" << e.what() << std::endl;
        return e.code().value();
    }
    return 0;
}

int send_data_by_write() {
    const auto raw_ip_address(std::string("100.65.140.179"));

    try {
        constexpr unsigned short port = 8089;
        const boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(raw_ip_address), port);
        boost::asio::io_context ioc;
        boost::asio::ip::tcp::socket sock(ioc, ep.protocol());

        sock.connect(ep);
        const auto buf(std::string("hello, buffer"));
        [[maybe_unused]] size_t write_length = boost::asio::write(sock, boost::asio::buffer(buf.c_str(), buf.size()));

    }catch(const boost::system::system_error& e) {
        std::cout << "Error code:" << e.code() << std::endl;
        std::cout << "Error message:" << e.what() << std::endl;
        return e.code().value();
    }
    return 0;
}

std::string read_from_socket(boost::asio::ip::tcp::socket& sock) {
    constexpr unsigned char MESSAGE_SIZE = 7;
    char buf[MESSAGE_SIZE];
    size_t total_bytes_read = 0;
    while (total_bytes_read != MESSAGE_SIZE) {
        total_bytes_read += sock.read_some(boost::asio::buffer(buf + total_bytes_read, MESSAGE_SIZE - total_bytes_read));
    }
    return {buf, total_bytes_read};
}

int read_data_by_read_some() {
    const auto raw_ip_address(std::string("100.65.140.179"));

    try {
        constexpr unsigned short port = 8080;
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(raw_ip_address), port);
        boost::asio::io_context ioc;
        boost::asio::ip::tcp::socket sock(ioc, ep.protocol());

        sock.connect(ep);
        write_to_socket(sock);

    }catch (const boost::system::system_error& e) {
        std::cout << "Error code:" << e.code() << std::endl;
        std::cout << "Error message:" << e.what() << std::endl;
        return e.code().value();
    }

    return 0;
}

int read_data_by_receive() {
    const auto raw_ip_address(std::string("100.65.140.179"));

    try {
        constexpr unsigned short port = 8080;
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(raw_ip_address), port);
        boost::asio::io_context ioc;
        boost::asio::ip::tcp::socket sock(ioc, ep.protocol());

        sock.connect(ep);
        constexpr unsigned char BUFF_SIZE = 7;
        char buffer[BUFF_SIZE];
        [[maybe_unused]] size_t receive_length = sock.receive(boost::asio::buffer(buffer, BUFF_SIZE));

    }catch (const boost::system::system_error& e) {
        std::cout << "Error code:" << e.code() << std::endl;
        std::cout << "Error message:" << e.what() << std::endl;
        return e.code().value();
    }

    return 0;
}

int read_data_by_read() {
    const auto raw_ip_address(std::string("100.65.140.179"));

    try {
        constexpr unsigned short port = 8080;
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(raw_ip_address), port);
        boost::asio::io_context ioc;
        boost::asio::ip::tcp::socket sock(ioc, ep.protocol());

        sock.connect(ep);
        constexpr unsigned char BUFF_SIZE = 7;
        char buffer[BUFF_SIZE];
        [[maybe_unused]] size_t receive_length = boost::asio::read(sock, boost::asio::buffer(buffer, BUFF_SIZE));

    }catch (const boost::system::system_error& e) {
        std::cout << "Error code:" << e.code() << std::endl;
        std::cout << "Error message:" << e.what() << std::endl;
        return e.code().value();
    }

    return 0;
}

