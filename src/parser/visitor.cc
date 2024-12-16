#include "visitor.h"

namespace SrSecurity::Parser {
std::any Visitor::visitInclude(Antlr4Gen::SecLangParser::IncludeContext* ctx) {
  std::string file_path = ctx->FILE_PATH()->getText();
  return parser_->loadFromFile(file_path);
}
} // namespace SrSecurity::Parser