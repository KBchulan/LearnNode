#ifndef CONST_HPP
#define CONST_HPP

#define MAX_LENGTH 2048   // 单个包的最大长度
#define HEAD_TOTAL_LEN 4  // 头部总长度
#define HEAD_ID_LEN 2     // ID长度
#define HEAD_DATA_LEN 2   // 信息长度（不是信息）
#define MAX_RECVQUE 10000 // 接收队列最大值
#define MAX_SENDQUE 1000  // 发送队列最大值

enum class MSG_IDS
{
    MSG_HELLO_WORLD = 1001
};

#endif // !CONST_HPP