#include "nigiri/loader/netex/line.h"

namespace nigiri::loader::netex {

void read_lines(timetable& tt,
                const pugi::xml_document& doc,
                hash_map<std::string_view, line>& line_map,
                hash_map<std::string_view, provider_idx_t>& operatorMap) {

  for (auto const& next_line : doc.select_nodes("//Line")) {
    auto id = next_line.node().attribute("id").value();
    auto name = next_line.node().child("Name").text().get();
    auto transport_mode = next_line.node().child("TransportMode").text().get();
    auto operator_ref = next_line.node()
                            .select_node("//OperatorRef")
                            .node()
                            .attribute("ref")
                            .value();

    provider_idx_t line_operator_idx;
    // Option 1: Provider already exists
    if (operatorMap.find(operator_ref) != operatorMap.end()) {
      line_operator_idx = operatorMap[operator_ref];
    } else {  // Option 2: Unknown provider
      line_operator_idx = tt.register_provider(
          {operator_ref, "UNKNOWN_AGENCY",
           timezone_idx_t{0}});  // TODO Timezone not fixed yet
    }

    // tt.register_route();
    line new_line{id, name, transport_mode, line_operator_idx};
    line_map.insert(std::make_pair(id, new_line));
  }
}
}  // namespace nigiri::loader::netex
