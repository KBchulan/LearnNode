//
// Created by whx on 24-11-10.
//

#ifndef LOGICNODE_HPP
#define LOGICNODE_HPP

#include <CSession.hpp>

class LogicNode
{
    friend class LogicSystem;

public:
    explicit LogicNode(std::shared_ptr<CSession> session, std::shared_ptr<RecvNode> recvnode);
    
    // 添加析构函数用于调试
    ~LogicNode() {
        std::cout << "LogicNode destruct" << std::endl;
    }

private:
    std::weak_ptr<CSession> _session;
    std::shared_ptr<RecvNode> _recvnode;
};

#endif // LOGICNODE_HPP
