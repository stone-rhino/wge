/**
 * Copyright (c) 2024-2026 Stone Rhino and contributors.
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
#include "inbound_data_error.h"

#include "../engine.h"

namespace Wge {
namespace Variable {

void InboundDataError::evaluateCollection(Transaction& t, Common::EvaluateResults& result) const {
  const auto& engine_config = t.getEngine().config();

  auto request_body_processor = t.getRequestBodyProcessor();
  if (request_body_processor == BodyProcessorType::Json ||
      request_body_processor == BodyProcessorType::UrlEncoded ||
      request_body_processor == BodyProcessorType::Xml) {
    result.emplace_back(t.getRequestBody().size() > engine_config.request_body_limit_);
  } else if (request_body_processor == BodyProcessorType::MultiPart) {
    result.emplace_back(t.getBodyMultiPart().getNoFilesSize() >
                        engine_config.request_body_no_files_limit_);
  }
}

} // namespace Variable
} // namespace Wge