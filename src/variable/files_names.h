#pragma once

#include "collection_base.h"
#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class FilesNames : public VariableBase, public CollectionBase {
  DECLARE_VIRABLE_NAME(FILES_NAMES);

public:
  FilesNames(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter), CollectionBase(sub_name_) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    auto& filename = t.getBodyMultiPart().getNameFileNameLinked();
    auto& filename_map = t.getBodyMultiPart().getNameFileName();

    RETURN_IF_COUNTER(
        // collection
        { result.append(static_cast<int>(filename.size())); },
        // specify subname
        {
          auto iter = filename_map.find(sub_name_);
          result.append(iter != t.getBodyMultiPart().getNameFileName().end() ? 1 : 0);
        });

    RETURN_VALUE(
        // collection
        {
          for (auto& elem : filename) {
            if (!hasExceptVariable(elem->first)) [[likely]] {
              result.append(elem->first);
            }
          }
        },
        // collection regex
        {
          for (auto& elem : filename) {
            if (!hasExceptVariable(elem->first)) [[likely]] {
              if (match(elem->first)) {
                result.append(elem->first);
              }
            }
          }
        },
        // specify subname regex
        {
          auto iter = filename_map.find(sub_name_);
          if (iter != filename_map.end()) {
            result.append(iter->first);
          }
        });
  };

  bool isCollection() const override { return sub_name_.empty(); };
};
} // namespace Variable
} // namespace SrSecurity