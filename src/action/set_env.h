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

#include "action_base.h"

#include "../macro/macro_base.h"

namespace Wge {
namespace Action {
/**
 * Creates and updates environment variables that can be accessed by both ModSecurity and the web
 * server.
 */
class SetEnv final : public ActionBase {
  DECLARE_ACTION_NAME(setenv);

public:
  SetEnv(std::string&& key, std::string&& value);
  SetEnv(std::string&& key, std::unique_ptr<Macro::MacroBase>&& macro);

public:
  void evaluate(Transaction& t) const override;

private:
  std::string key_;
  std::string value_;
  const std::unique_ptr<Macro::MacroBase> macro_;
};
} // namespace Action
} // namespace Wge