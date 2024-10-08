#include "behaviortree/control/fallback_node.hpp"

namespace behaviortree {
FallbackNode::FallbackNode(const std::string &rName, bool makeAsynch): ControlNode::ControlNode(rName, {}),
                                                                       m_curChildIdx(0),
                                                                       m_asynch(makeAsynch) {
    if(m_asynch) {
        SetRegistrationId("AsyncFallback");
    } else {
        SetRegistrationId("Fallback");
    }
}

NodeStatus FallbackNode::Tick() {
    const size_t childrenNum = m_childrenNodeVec.size();

    if(GetNodeStatus() == NodeStatus::Idle) {
        m_skippedNum = 0;
    }

    SetNodeStatus(NodeStatus::Running);

    while(m_curChildIdx < childrenNum) {
        TreeNode *pCurChildNode = m_childrenNodeVec[m_curChildIdx];

        auto preNodeStatus = pCurChildNode->GetNodeStatus();
        const NodeStatus childNodeStatus = pCurChildNode->ExecuteTick();

        switch(childNodeStatus) {
            case NodeStatus::Running: {
                return childNodeStatus;
            } break;
            case NodeStatus::Success: {
                ResetChildren();
                m_curChildIdx = 0;
                return childNodeStatus;
            } break;
            case NodeStatus::Failure: {
                m_curChildIdx++;
                // Return the execution flow if the child is async,
                // to make this interruptable.
                if(m_asynch and RequiresWakeUp() and preNodeStatus == NodeStatus::Idle and m_curChildIdx < childrenNum) {
                    EmitWakeUpSignal();
                    return NodeStatus::Running;
                }
            } break;
            case NodeStatus::Skipped: {
                // It was requested to skip this node
                m_curChildIdx++;
                m_skippedNum++;
            } break;
            case NodeStatus::Idle: {
                throw util::LogicError("[", GetNodeName(), "]: A children should not return IDLE");
            } break;
            default: {
            } break;
        }// end switch
    }// end while loop

    // The entire while loop completed. This means that all the children returned FAILURE.
    if(m_curChildIdx == childrenNum) {
        ResetChildren();
        m_curChildIdx = 0;
    }

    // Skip if ALL the nodes have been skipped
    return (m_skippedNum == childrenNum) ? NodeStatus::Skipped : NodeStatus::Failure;
}

void FallbackNode::Halt() {
    m_curChildIdx = 0;
    ControlNode::Halt();
}

}// namespace behaviortree
