#ifndef BEHAVIORTREE_FORCE_SUCCESS_NODE_H
#define BEHAVIORTREE_FORCE_SUCCESS_NODE_H

#include "behaviortree/decorator_node.h"

namespace behaviortree {
/**
 * @brief The ForceSuccessNode returns always SUCCESS or RUNNING.
 */
class ForceSuccessNode: public DecoratorNode {
 public:
    ForceSuccessNode(const std::string &refName): DecoratorNode(refName, {}) {
        SetRegistrationId("ForceSuccess");
    }

 private:
    virtual behaviortree::NodeStatus Tick() override;
};

//------------ implementation ----------------------------

inline NodeStatus ForceSuccessNode::Tick() {
    SetNodeStatus(NodeStatus::Running);

    const NodeStatus childStatus = m_ChildNode->ExecuteTick();

    if(IsNodeStatusCompleted(childStatus)) {
        ResetChild();
        return NodeStatus::Success;
    }

    // RUNNING or skipping
    return childStatus;
}
}// namespace behaviortree

#endif// BEHAVIORTREE_FORCE_SUCCESS_NODE_H
