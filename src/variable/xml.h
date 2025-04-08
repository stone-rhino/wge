#pragma once

#include "collection_base.h"
#include "evaluate_help.h"
#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class Xml : public VariableBase, public CollectionBase {
  DECLARE_VIRABLE_NAME(XML);

public:
  Xml(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter), CollectionBase(sub_name_) {
    if (sub_name_ == "//@*") {
      type_ = Type::AttrValue;
    }
  }

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    const std::vector<std::string_view>* values = nullptr;
    if (type_ == Type::AttrValue) {
      values = &(t.getBodyXml().getAttrValues());
    } else {
      values = &(t.getBodyXml().getTagValues());
    }

    RETURN_IF_COUNTER(
        // collection
        { result.append(static_cast<int>(values->size())); },
        // specify subname
        { result.append(static_cast<int>(values->size())); });

    RETURN_VALUE(
        // collection
        {
          for (auto& elem : *values) {
            result.append(elem);
          }
        },
        // collection regex
        {
          UNREACHABLE();
        },
        // specify subname
        {
          for (auto& elem : *values) {
            result.append(elem);
          }
        });
  };

  bool isCollection() const override { return sub_name_.empty(); };

private:
  enum class Type {
    AttrValue,
    TagValue,
  };

  Type type_{Type::TagValue};
};
} // namespace Variable
} // namespace SrSecurity