//
// Created by whx on 24-11-10.
//

#ifndef LOGICNODE_HPP
#define LOGICNODE_HPP

#include <CSession.hpp>

class LogicNode {
    friend class LogicSystem;

public:
    explicit LogicNode(const std::shared_ptr<CSession> &session, const std::shared_ptr<RecvNode> &recvnode);

private:
    std::shared_ptr<CSession> _session;
    std::shared_ptr<RecvNode> _recvnode;
};

#endif //LOGICNODE_HPP
