#ifndef BEHAVIORTREE_JSON_PARSING_H
#define BEHAVIORTREE_JSON_PARSING_H

#include <filesystem>
#include <unordered_map>

#include "behaviortree/parser.h"
#include "nlohmann/json.hpp"

namespace behaviortree {
/**
 * @brief The JsonParser is a class used to read the model
 * of a BehaviorTree from file or text and instantiate the
 * corresponding tree using the BehaviorTreeFactory.
 */
class JsonParser: public Parser {
 public:
    JsonParser(const BehaviorTreeFactory &rFactory);

    ~JsonParser() override;

    JsonParser(const JsonParser &rOther) = delete;
    JsonParser &operator=(const JsonParser &rOther) = delete;

    JsonParser(JsonParser &&rOther) noexcept;
    JsonParser &operator=(JsonParser &&rOther) noexcept;

    void LoadFromFile(const std::filesystem::path &rFilepath, bool addInclude = true) override;

    void LoadFromText(const std::string &rText, bool addInclude = true) override;

    [[nodiscard]] std::vector<std::string> GetRegisteredTreeName() const override;

    [[nodiscard]] Tree InstantiateTree(const Blackboard::Ptr &rRootBlackboard, std::string maintreeToExecute = {}) override;

    void ClearInternalState() override;

 private:
    struct PImpl;
    std::unique_ptr<PImpl> m_pPImpl;
};

void VerifyJson(const std::string &rJsonText, const std::unordered_map<std::string, NodeType> &rRegisteredNodes);

/**
 * @brief WriteTreeNodeModelJson generates an Json that contains the GetManifest in the
 * <TreeNodeModel>
 *
 * @param rFactory          the factory with the registered types
 * @param includeBuiltin    if true, include the builtin Nodes
 *
 * @return  string containing the Json.
 */
[[nodiscard]] std::string WriteTreeNodeModelJson(const BehaviorTreeFactory &rFactory, bool includeBuiltin = false);

/**
 * @brief WriteTreeToJson create a string that contains the Json that corresponds to a given tree.
 * When using this function with a logger, you should probably set both add_metadata and
 * add_builtin_models to true.
 *
 * @param rTree               the input tree
 * @param addMetadata         if true, the attributes "_uid" and "_fullPath" will be added to the nodes
 * @param addBuiltinModels    if true, include the builtin Nodes into the <TreeNodeModel>
 *
 * @return string containing the Json.
 */
[[nodiscard]] std::string WriteTreeToJson(const Tree &rTree, bool addMetadata, bool addBuiltinModels);

} // namespace behaviortree

#endif // BEHAVIORTREE_JSON_PARSING_H
