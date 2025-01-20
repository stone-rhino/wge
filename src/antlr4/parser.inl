#include "parser.h"

namespace SrSecurity::Antlr4 {
std::unordered_map<std::string, Rule::Severity> Parser::serverity_factory_ = {
    {"EMERGENCY", Rule::Severity::EMERGENCY}, {"ALERT", Rule::Severity::ALERT},
    {"CRITICAL", Rule::Severity::CRITICAL},   {"ERROR", Rule::Severity::ERROR},
    {"WARNING", Rule::Severity::WARNING},     {"NOTICE", Rule::Severity::NOTICE},
    {"INFO", Rule::Severity::INFO},           {"DEBUG", Rule::Severity::DEBUG},
};

} // namespace SrSecurity::Antlr4