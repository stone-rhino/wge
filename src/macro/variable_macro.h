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

#include "macro_base.h"
#include "memory"

#include "../common/log.h"
#include "../variable/variable_base.h"

namespace Wge {
namespace Macro {
class VariableMacro final : public MacroBase {
public:
  VariableMacro(std::string&& literal_value, std::unique_ptr<Variable::VariableBase>&& variable)
      : MacroBase(std::move(literal_value)), variable_(std::move(variable)) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    variable_->evaluate(t, result);
    WGE_LOG_TRACE("macro %{{{}}} expanded: {}", makeVariableName(),
                  VISTIT_VARIANT_AS_STRING(result.front().variant_));
  }

public:
  const std::unique_ptr<Variable::VariableBase>& getVariable() const { return variable_; }

private:
  std::string makeVariableName() const {
    std::string name;
    name = variable_->mainName();
    if (!variable_->subName().empty()) {
      name += "." + variable_->subName();
    }
    return name;
  }

private:
  const std::unique_ptr<Variable::VariableBase> variable_;
};
} // namespace Macro
} // namespace Wge