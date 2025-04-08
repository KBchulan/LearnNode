#include "Socket.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <array>
#include <cstdio>
#include <cstring>
#include <middleware/Logger.hpp>

namespace core {

void SocketCall::socketClientCall() noexcept {
  int sock = 0;
  struct sockaddr_in serv_addr;
  std::array<char, 1024> buffer = {0};

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    logger.error("Socket创建失败");
    return;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(8080);

  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    logger.error("地址转换失败");
    return;
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    logger.error("连接失败");
    return;
  }

  const char *message = "你好，服务器";
  send(sock, message, strlen(message), 0);
  logger.info("消息已发送");

  read(sock, buffer.data(), 1024);
  logger.info("服务器响应: {}", buffer.data());

  close(sock);
}

void SocketCall::socketServerCall() noexcept {
  int server_fd;
  int new_socket;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  std::array<char, 1024> buffer = {0};

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    logger.error("socket创建失败");
    return;
  }

  if ((setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                  sizeof(opt))) < 0) {
    logger.error("setsockopt设置失败");
    return;
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(8080);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    logger.error("bind失败");
    return;
  }

  if (listen(server_fd, 3) < 0) {
    logger.error("listen失败");
    return;
  }

  logger.info("等待客户端连接...");

  if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                           (socklen_t *)&addrlen)) < 0) {
    logger.error("accept失败");
    return;
  }

  read(new_socket, buffer.data(), 1024);
  logger.info("客户端消息: {}", buffer.data());

  const char *hello = "服务器已收到消息";
  send(new_socket, hello, strlen(hello), 0);
  logger.info("响应消息已发送");

  close(new_socket);
  close(server_fd);
}

void SocketCall::socketpairCall() noexcept {
  std::array<int, 2> socket_pair;

  int ret = socketpair(AF_UNIX, SOCK_STREAM, 0, socket_pair.data());
  if (ret < 0) {
    logger.error("socketpair创建失败");
    return;
  }  

  // 在fork()之前需要打开socketpair()
  // pid_t cpid = fork();
}

}  // namespace core