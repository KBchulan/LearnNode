//
// Created by whx on 24-11-10.
//

#include "../include/LogicSystem.hpp"

#include <config.hpp>
#include <jsoncpp/json/reader.h>

LogicSystem::~LogicSystem()
{
    _b_stop = true;
    _consume.notify_all();
    _worker_thread.join();
}

void LogicSystem::PostMsgToQueue(const std::shared_ptr<LogicNode> &msg)
{
    std::unique_lock unique_locker(_mutex);
    _msg_que.push(msg);

    if (_msg_que.size() == 1)
        _consume.notify_one();
}

LogicSystem::LogicSystem() : _b_stop(false)
{
    RegisterCallbacks();
    _worker_thread = std::thread(&LogicSystem::DealMsg, this);
}

void LogicSystem::DealMsg()
{
    while (true)
    {
        std::unique_lock unique_locker(_mutex);

        // 判断队列为空，则用条件变量阻塞
        if (_msg_que.empty() && !_b_stop)
        {
            _consume.wait(unique_locker);
        }

        // 判断如果为关闭状态，取出所有的进行处理并退出循环
        if (_b_stop)
        {
            while (!_msg_que.empty())
            {
                const auto msg_node = _msg_que.front();
                std::cout << R"(Recv msg id is: )" << msg_node->_recvnode->GetMsgId() << '\n';

                auto call_back_iter = _fun_callbak.find(msg_node->_recvnode->GetMsgId());
                if (call_back_iter == _fun_callbak.end())
                {
                    _msg_que.pop();
                    continue;
                }

                call_back_iter->second(msg_node->_session, msg_node->_recvnode->GetMsgId(), std::string(msg_node->_recvnode->GetData(), msg_node->_recvnode->_total_len));
                _msg_que.pop();
            }
            break;
        }

        // 队列不空了
        const auto msg_node = _msg_que.front();
        std::cout << R"(Recv msg id is: )" << msg_node->_recvnode->GetMsgId() << '\n';

        auto call_back_iter = _fun_callbak.find(msg_node->_recvnode->GetMsgId());
        if (call_back_iter == _fun_callbak.end())
        {
            _msg_que.pop();
            continue;
        }

        call_back_iter->second(msg_node->_session, msg_node->_recvnode->GetMsgId(), std::string(msg_node->_recvnode->GetData(), msg_node->_recvnode->_total_len));
        _msg_que.pop();
    }
}

void LogicSystem::RegisterCallbacks()
{
    _fun_callbak[MSG_HELLO_WORLD] = [this]<typename T0, typename T1, typename T2>(T0 &&PH1, T1 &&PH2, T2 &&PH3)
    {
        HelloWorldCallback(std::forward<T0>(PH1), std::forward<T1>(PH2), std::forward<T2>(PH3));
    };
}

void LogicSystem::HelloWorldCallback(const std::shared_ptr<CSession> &session, const long &msg_id,
                                     const std::string &msg_data)
{
    Json::Reader reader;
    Json::Value root;
    reader.parse(msg_data, root);
    std::cout << "Receive msg id is: " << root["id"].asInt() << '\n';
    std::cout << "Receive msg body is: " << root["body"].asString() << '\n';
    root["body"] = "Server has received msg, msg body is: " + root["body"].asString();

    const std::string return_str = root.toStyledString();
    session->Send(return_str, msg_id);
}
