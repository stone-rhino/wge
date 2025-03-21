/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2025 Trustwave Holdings, Inc. (http://www.trustwave.com/)
 *
 * Modifications made by Stone Rhino, 2025.
 *
 * You may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 */
#pragma once

#include <string>
#include <string_view>

namespace SrSecurity::Transformation::ModSecurity {
std::string cmdLime(std::string_view data);
std::string cssDecode(std::string_view data);
} // namespace SrSecurity::Transformation::ModSecurity
