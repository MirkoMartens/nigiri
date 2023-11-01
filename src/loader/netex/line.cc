#include "nigiri/loader/netex/line.h"

namespace nigiri::loader::netex {

void read_lines(const pugi::xml_document& doc,
                hash_map<std::string_view, line>& line_map,
                hash_map<std::string_view, provider_idx_t>& operatorMap) {
  for (const auto& next_line : doc.select_nodes("//Line")) {
    auto id = next_line.node().attribute("id").value();
    auto name = next_line.node().child("Name").text().get();
    auto transport_mode = next_line.node().child("TransportMode").text().get();
    auto operator_ref = next_line.node()
                            .select_node("//OperatorRef")
                            .node()
                            .attribute("ref")
                            .value();
    auto operator_name = operatorMap[operator_ref];

    line new_line{id, name, transport_mode, operator_name};
    line_map.insert(std::make_pair(id, new_line));
  }
}
}  // namespace nigiri::loader::netex
