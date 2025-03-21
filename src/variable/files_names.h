#pragma once

#include "collection_base.h"
#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class FilesNames : public VariableBase, public CollectionBase {
  DECLARE_VIRABLE_NAME(FILES_NAMES);

public:
  FilesNames(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    assert(false);
    throw "Not implemented!";
  };

  bool isCollection() const override { return sub_name_.empty(); };
};
} // namespace Variable
} // namespace SrSecurity