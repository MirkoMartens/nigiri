#pragma once

#include "nigiri/timetable.h"
#include <pugixml.hpp>
#include <string_view>
namespace nigiri::loader::netex {

struct site_connection {
  std::string_view id;
  std::string_view walk_transfer_duration;
  std::string_view stop_place_idx_from;
  std::string_view quay_idx_from;
  std::string_view stop_place_idx_to;
  std::string_view quay_idx_to;
};

void read_site_connections(
    const timetable& tt,
    const pugi::xml_document& doc,
    hash_map<std::string_view, site_connection>& connection_map);

}  // namespace nigiri::loader::netex