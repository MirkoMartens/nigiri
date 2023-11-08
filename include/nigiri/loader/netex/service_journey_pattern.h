#pragma once

#include <string_view>
#include <vector>
#include "line.h"
#include "scheduled_stop_point.h"

namespace nigiri::loader::netex {

struct stop_point_in_journey {
  std::string_view id_;
  scheduled_stop_point ssp_;
  bool for_alighting_;
  bool for_boarding_;
  std::string_view front_display_text_;  // DestinationDisplayView->FrontText
};

struct service_journey_pattern {
  std::string_view id_;
  std::string_view line_name_;
  std::vector<stop_point_in_journey> points_in_sequence_;
  std::string_view start_point_id_;
  std::string_view end_point_id_;
};

void read_journey_patterns(
    const pugi::xml_document& doc,
    hash_map<std::string_view, service_journey_pattern>& journeys_map,
    hash_map<std::string_view, line>& lines_map,
    hash_map<std::string_view, scheduled_stop_point>& ssp_map);
}  // namespace nigiri::loader::netex