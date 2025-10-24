#ifndef LOGICSYSTEM_HPP
#define LOGICSYSTEM_HPP

#include "const.hpp"

#include <map>
#include <queue>
#include <thread>
#include <functional>
#include <condition_variable>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/reader.h>

class CSession;
class LogicNode;
class LogicSystem
{
    using FunCallBack = std::function<void(std::shared_ptr<CSession> session, const short &msg_id, const std::string &msg_data)>;

public:
    ~LogicSystem();

    LogicSystem(const LogicSystem &) = delete;
    LogicSystem &operator=(const LogicSystem &) = delete;

    // 将信息投递到队列里
    void PostMsgToQue(std::shared_ptr<LogicNode> msg);

    // 获取实例
    static LogicSystem &GetInstance();

private:
    LogicSystem();

    // 逻辑线程函数
    void DealMsg();

    // 所有回调集合
    void RegisterCallBacks();

    // hello的回调
    static void HelloCallBack(std::shared_ptr<CSession> session, const short &msg_id, const std::string &msg_data);

private:

    bool _stop;
    std::mutex _mutex;
    std::thread _worker_thread;
    std::condition_variable _consume;
    std::map<short, FunCallBack> _fun_callbacks;
    std::queue<std::shared_ptr<LogicNode>> _msg_que;
};

#endif // !LOGICSYSTEM_HPP