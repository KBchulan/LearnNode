//
// Created by whx on 24-11-10.
//

#include <../include/LogicNode.hpp>

LogicNode::LogicNode(std::shared_ptr<CSession> session,
                    std::shared_ptr<RecvNode> recvnode) : _session(session),
                                                         _recvnode(std::move(recvnode))
{
}
