#include "behaviortree/scripting/script_parser.hpp"

#include <lexy/action/parse.hpp>
#include <lexy/action/validate.hpp>
#include <lexy/code_point.hpp>
#include <lexy/input/string_input.hpp>
#include <lexy_ext/report_error.hpp>

#include "behaviortree/scripting/operators.hpp"

namespace behaviortree {

using ErrorReport = lexy_ext::_report_error<char *>;

Expected<ScriptFunction> ParseScript(const std::string &script) {
    char error_msgs_buffer[2048];

    auto input = lexy::string_input<lexy::utf8_encoding>(script);
    auto result = lexy::parse<behaviortree::Grammar::stmt>(
            input, ErrorReport().to(error_msgs_buffer)
    );
    if(result.has_value() and result.error_count() == 0) {
        try {
            std::vector<behaviortree::Ast::ExprBase::Ptr> exprs =
                    LEXY_MOV(result).value();
            if(exprs.empty()) {
                return nonstd::make_unexpected("Empty Script");
            }

            return [exprs, script](Ast::Environment &env) -> Any {
                try {
                    for(auto i = 0u; i < exprs.size() - 1; ++i) {
                        exprs[i]->evaluate(env);
                    }
                    return exprs.back()->evaluate(env);
                } catch(util::RuntimeError &err) {
                    throw util::RuntimeError(util::StrCat(
                            "Error in script [", script, "]\n", err.what()
                    ));
                }
            };
        } catch(std::runtime_error &err) {
            return nonstd::make_unexpected(err.what());
        }
    } else {
        return nonstd::make_unexpected(error_msgs_buffer);
    }
}

behaviortree::Expected<Any> ParseScriptAndExecute(
        Ast::Environment &env, const std::string &script
) {
    auto executor = ParseScript(script);
    if(executor) {
        return executor.value()(env);
    } else// forward the error
    {
        return nonstd::make_unexpected(executor.error());
    }
}

Result ValidateScript(const std::string &script) {
    char error_msgs_buffer[2048];

    auto input = lexy::string_input<lexy::utf8_encoding>(script);
    auto result = lexy::parse<behaviortree::Grammar::stmt>(
            input, ErrorReport().to(error_msgs_buffer)
    );
    if(result.has_value() and result.error_count() == 0) {
        try {
            std::vector<behaviortree::Ast::ExprBase::Ptr> exprs =
                    LEXY_MOV(result).value();
            if(exprs.empty()) {
                return nonstd::make_unexpected("Empty Script");
            }
            // valid script
            return {};
        } catch(std::runtime_error &err) {
            return nonstd::make_unexpected(err.what());
        }
    }
    return nonstd::make_unexpected(error_msgs_buffer);
}

}// namespace behaviortree
