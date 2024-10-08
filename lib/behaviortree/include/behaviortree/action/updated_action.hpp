#ifndef BEHAVIORTREE_UPDATED_ACTION_HPP
#define BEHAVIORTREE_UPDATED_ACTION_HPP

#include "behaviortree/action_node.h"
#include "behaviortree/common.h"

namespace behaviortree {
/**
 * @brief The EntryUpdatedAction checks the Timestamp in an entry
 * to determine if the value was updated since the last time.
 *
 * SUCCESS if it was updated, since the last time it was checked,
 * FAILURE if it doesn't exist or was not updated.
 */
class BEHAVIORTREE_API EntryUpdatedAction: public SyncActionNode {
 public:
    EntryUpdatedAction(const std::string &rName, const NodeConfig &rConfig);

    ~EntryUpdatedAction() override = default;

    static PortMap ProvidedPorts() {
        return {InputPort<behaviortree::Any>("entry", "Entry to check")};
    }

 private:
    uint64_t m_sequenceId{0};
    std::string m_entryKey;

    NodeStatus Tick() override;
};

}// namespace behaviortree

#endif// BEHAVIORTREE_UPDATED_ACTION_HPP
