#pragma once

#include "nigiri/types.h"
#include <pugixml.hpp>
#include <string_view>

namespace nigiri::loader::netex {
struct stop_assignment {
  std::string_view id;
  std::string_view ssp_ref;
  std::string_view stop_palce_ref;
};

void read_stop_assignments(
    const pugi::xml_document& doc,
    hash_map<std::string_view, stop_assignment> stop_assignment_map);
}  // namespace nigiri::loader::netex