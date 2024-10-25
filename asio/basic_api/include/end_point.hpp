//
// Created by whx on 24-10-24.
//

#ifndef END_POINT_HPP
#define END_POINT_HPP
#include <boost/asio/detail/descriptor_ops.hpp>
#include <boost/asio/ip/address.hpp>

// 端点是服务器端用来绑定，客户端用来连接的东西，可以说是媒介

// 客户端端点
extern int client_end_point();

// 服务端端点
extern int server_end_point();

// TCP的socket
extern int create_tcp_socket();

// accept的socket
extern int create_accept_socket();

// 让服务器的accept绑定端口和ip
extern int bind_accept_socket();

// 连接操作(地址版本)
extern int connect_to_endpoint();

// 连接操作(域名版本)
extern int dns_connect_to_endpoint();

// accept new connection
extern int accept_new_connection();

// 传递const_buffer_sequence（麻烦版本）
extern void use_const_buffer();

// 比较简单的使用方法
extern void use_buffer_str();

// 数组的处理方式
extern void use_buffer_array();

#endif //END_POINT_HPP
