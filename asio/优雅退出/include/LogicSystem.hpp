//
// Created by whx on 24-11-10.
//

#ifndef LOGICSYSTEM_HPP
#define LOGICSYSTEM_HPP

#include <map>
#include <queue>
#include <thread>
#include <functional>
#include <LogicNode.hpp>
#include <Singleton.hpp>
#include <jsoncpp/json/json.h>

class LogicSystem final : public Singleton<LogicSystem> {
    using FunCallBack = std::function<void(std::shared_ptr<CSession>, const long &msg_id, const std::string &msg_data)>;

    friend class Singleton<LogicSystem>;

public:
    ~LogicSystem();

    // 将逻辑消息放在队列里
    void PostMsgToQueue(const std::shared_ptr<LogicNode>& msg);

private:
    LogicSystem();

    // 消费者线程的函数
    void DealMsg();

    // 注册回调
    void RegisterCallbacks();

    // 第一个回调
    void HelloWorldCallback(const std::shared_ptr<CSession>&, const long &msg_id, const std::string &msg_data);

    bool _b_stop;
    std::mutex _mutex;
    std::thread _worker_thread; // 取队列里回调的线
    std::condition_variable _consume;
    std::map<long, FunCallBack> _fun_callbak;
    std::queue<std::shared_ptr<LogicNode>> _msg_que;
};

#endif //LOGICSYSTEM_HPP
