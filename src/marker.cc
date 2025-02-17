#include "marker.h"

namespace SrSecurity {
Marker::Marker(std::string&& name, const Rule* prev_rule)
    : name_(std::move(name)), prev_rule_(prev_rule) {}
} // namespace SrSecurity