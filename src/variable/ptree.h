/**
 * Copyright (c) 2024-2025 Stone Rhino and contributors.
 *
 * MIT License (http://opensource.org/licenses/MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#pragma once

#include "variable_base.h"

#include "../common/property_tree.h"

namespace Wge {
namespace Variable {
class PTree final : public VariableBase {
  DECLARE_VIRABLE_NAME(PTREE);

public:
  struct Path {
    enum class Type : uint8_t { Map, Array, Value };
    enum class Flag : uint8_t { Single, And, Or };
    std::string name_;
    Type type_;
    Flag flag_;
  };

public:
  PTree(std::string&& sub_name, bool is_not, bool is_counter, std::string_view curr_rule_file_path)
      : VariableBase(std::move(sub_name), is_not, is_counter) {
    initPaths(sub_name_, paths_);
    if (!paths_.empty()) {
      if (paths_.back().type_ == Path::Type::Array || paths_.back().type_ == Path::Type::Map) {
        is_collection_ = true;
      } else {
        for (auto& path : paths_) {
          if (path.type_ == Path::Type::Array) {
            is_collection_ = true;
            break;
          }
        }
      }
    }
  }

protected:
  void evaluateCollectionCounter(Transaction& t, Common::EvaluateResults& result) const override {
    Common::EvaluateResults temp_result;
    evaluateCollection(t, temp_result);
    result.emplace_back(static_cast<int64_t>(temp_result.size()));
  }

  void evaluateSpecifyCounter(Transaction& t, Common::EvaluateResults& result) const override {
    evaluateCollectionCounter(t, result);
  }

  void evaluateCollection(Transaction& t, Common::EvaluateResults& result) const override {
    const Common::PropertyTree* root = t.propertyTree();
    assert(root != nullptr);
    if (root) {
      if (paths_.empty()) {
        evaluateNode(root, result);
      } else {
        evaluateNode(root, paths_, 0, result);
      }
    }
  }

  void evaluateSpecify(Transaction& t, Common::EvaluateResults& result) const override {
    evaluateCollection(t, result);
  }

public:
  bool isCollection() const override { return is_collection_; }
  const std::vector<Path>& paths() const { return paths_; }

public:
  static void initPaths(const std::string& sub_name, std::vector<Path>& paths);

  static void evaluateNode(const Common::PropertyTree* node, const std::vector<Path>& paths,
                           size_t path_index, Common::EvaluateResults& result);
  static void evaluateNode(const Common::PropertyTree* node, Common::EvaluateResults& result);

private:
  std::vector<Path> paths_;
  bool is_collection_{false};
};
} // namespace Variable
} // namespace Wge