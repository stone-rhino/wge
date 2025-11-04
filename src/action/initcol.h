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
#include "../persistent_storage/storage.h"

namespace Wge {
namespace Action {
class InitCol final : public ActionBase {
  DECLARE_ACTION_NAME(initcol);

public:
  InitCol(PersistentStorage::Storage::Type type, std::string&& key, std::string&& value);
  InitCol(PersistentStorage::Storage::Type type, std::string&& key,
          std::unique_ptr<Macro::MacroBase>&& value);

public:
  void evaluate(Transaction& t) const override;

private:
  std::string key_;
  std::string value_;
  const std::unique_ptr<Macro::MacroBase> value_macro_;
  PersistentStorage::Storage::Type type_;
};
} // namespace Action
} // namespace Wge