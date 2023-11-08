#pragma once

#include "nigiri/timetable.h"
#include <pugixml.hpp>
#include <string_view>
namespace nigiri::loader::netex {

struct site_connection {
  std::string_view id_;
  std::string_view walk_transfer_duration_;
  std::string_view stop_place_idx_from_;
  std::string_view quay_idx_from_;
  std::string_view stop_place_idx_to_;
  std::string_view quay_idx_to_;
};

void read_site_connections(
    const timetable& tt,
    const pugi::xml_document& doc,
    hash_map<std::string_view, site_connection>& connection_map);

}  // namespace nigiri::loader::netex