#ifndef BEHAVIORTREE_WHILE_DO_ELSE_NODE_HPP
#define BEHAVIORTREE_WHILE_DO_ELSE_NODE_HPP

#include "behaviortree/common.h"
#include "behaviortree/control_node.h"

namespace behaviortree {
/**
 * @brief WhileDoElse must have exactly 2 or 3 GetChildrenNode.
 * It is a REACTIVE node of IfThenElseNode.
 *
 * The first GetChildNode is the "statement" that is executed at each tick
 *
 * If result is SUCCESS, the second GetChildNode is executed.
 *
 * If result is FAILURE, the third GetChildNode is executed.
 *
 * If the 2nd or 3d GetChildNode is RUNNING and the statement changes,
 * the RUNNING GetChildNode will be stopped before starting the sibling.
 *
 */
class BEHAVIORTREE_API WhileDoElseNode: public ControlNode {
 public:
    WhileDoElseNode(const std::string &rName);

    virtual ~WhileDoElseNode() override = default;

    virtual void Halt() override;

 private:
    virtual behaviortree::NodeStatus Tick() override;
};

}// namespace behaviortree

#endif// BEHAVIORTREE_WHILE_DO_ELSE_NODE_HPP
