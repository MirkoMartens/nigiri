#pragma once

#include "nigiri/types.h"
#include <pugixml.hpp>
#include <string_view>

namespace nigiri::loader::netex {
struct line {
  std::string_view id;  // The id we use within the xml file to match it
  // std::string_view global_id;  // example: de:rmv:00004142 -> No idea why tho
  std::string_view name;
  std::string_view transport_mode;
  provider_idx_t line_operator;
};

void read_lines(const pugi::xml_document& doc,
                hash_map<std::string_view, line>& line_map,
                hash_map<std::string_view, provider_idx_t>& operatorMap);
}  // namespace nigiri::loader::netex