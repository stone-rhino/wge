#include "marker.h"

namespace SrSecurity {
Marker::Marker(std::string&& name, std::array<const Rule*, phase_total_>&& prev_rules)
    : name_(std::move(name)), prev_rules_(std::move(prev_rules)) {}
} // namespace SrSecurity