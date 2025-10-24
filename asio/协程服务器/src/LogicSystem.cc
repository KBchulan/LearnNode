#include "../include/CSession.hpp"
#include "../include/LogicSystem.hpp"

LogicSystem::LogicSystem() : _stop(false)
{
    RegisterCallBacks();

    _worker_thread = std::thread(&LogicSystem::DealMsg, this);
}

LogicSystem::~LogicSystem()
{
    _stop = true;
    _consume.notify_one();
    _worker_thread.join();
}

void LogicSystem::PostMsgToQue(std::shared_ptr<LogicNode> msg)
{
    std::unique_lock<std::mutex> lock(_mutex);
    _msg_que.push(msg);

    if(_msg_que.size() == 1)
    {
        _consume.notify_one();
    }
}

LogicSystem &LogicSystem::GetInstance()
{
    static LogicSystem instance;
    return instance;
}

void LogicSystem::DealMsg()
{
    while(true)
    {
        std::unique_lock<std::mutex> lock(_mutex);

        while(_msg_que.empty() && !_stop)
        {
            _consume.wait(lock);
        }

        if (_stop)
        {
            while(!_msg_que.empty())
            {
                auto msg_node = _msg_que.front();
                std::cout << "recv msg id is: " << msg_node->_recvnode->GetMsgId() << '\n';

                if(_fun_callbacks.find(msg_node->_recvnode->GetMsgId()) == _fun_callbacks.end())
                {
                    _msg_que.pop();
                    continue;
                }

                _fun_callbacks.find(msg_node->_recvnode->GetMsgId())->second(msg_node->_session, msg_node->_recvnode->GetMsgId(), std::string(msg_node->_recvnode->_data, msg_node->_recvnode->_total_len));
                _msg_que.pop();
            }
            break;
        }

        // 有数据传入
        auto msg_node = _msg_que.front();
        std::cout << "recv msg id is: " << msg_node->_recvnode->GetMsgId() << '\n';
        if(_fun_callbacks.find(msg_node->_recvnode->GetMsgId()) == _fun_callbacks.end())
        {
            _msg_que.pop();
            continue;
        }
        
        _fun_callbacks.find(msg_node->_recvnode->GetMsgId())->second(msg_node->_session, msg_node->_recvnode->GetMsgId(), std::string(msg_node->_recvnode->_data, msg_node->_recvnode->_total_len));
        _msg_que.pop();
    }
}

void LogicSystem::RegisterCallBacks()
{
    _fun_callbacks.emplace(static_cast<short>(MSG_IDS::MSG_HELLO_WORLD), HelloCallBack);
}

void LogicSystem::HelloCallBack(std::shared_ptr<CSession> session, const short &msg_id, const std::string &msg_data)
{
    Json::Reader reader;
    Json::Value root;

    reader.parse(msg_data, root);
    std::cout << "receive msg id is: " << root["id"].asInt() << '\n'
              << "receive msg data is: " << root["data"].asString() << '\n';

    root["data"] = "server has received msg, msg data is: " + root["data"].asString();
    
    std::string return_str = root.toStyledString();
    session->Send(return_str, root["id"].asInt());
}