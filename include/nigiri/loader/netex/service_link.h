#pragma once

#include "nigiri/types.h"
#include <pugixml.hpp>
#include <string_view>

namespace nigiri::loader::netex {
struct service_link {
  std::string_view id;
  std::string_view from_point_ref;
  std::string_view to_point_ref;
};

void read_service_links(
    const pugi::xml_document& doc,
    hash_map<std::string_view, service_link>& service_links_map);

}  // namespace nigiri::loader::netex