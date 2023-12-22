#pragma once

#include "nigiri/types.h"
#include <pugixml.hpp>
#include <string_view>
#include "scheduled_stop_point.h"
#include "stop_place.h"

namespace nigiri::loader::netex {
struct stop_assignment {
  std::string_view id_;
  std::string_view ssp_ref_;
  std::string_view stop_place_ref_;
};

void read_stop_assignments(
    const pugi::xml_document& doc,
    source_idx_t& src,
    hash_map<std::string_view, stop_assignment>& stop_assignment_map,
    hash_map<std::string_view, stop_place>& stop_map,
    hash_map<std::string_view, scheduled_stop_point>& ssp_map,
    timetable& tt);
}  // namespace nigiri::loader::netex