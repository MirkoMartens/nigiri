#include "nigiri/loader/netex/site_connection.h"

namespace nigiri::loader::netex {
void read_site_connections(
    const timetable& tt,
    const pugi::xml_document& doc,
    hash_map<std::string_view, site_connection>& connection_map) {

  for (auto const& sc : doc.select_nodes("//SiteConnection")) {
    const auto id = sc.node().attribute("id").value();

    const auto walk_transfer_duration =
        sc.node().select_node("//DefaultDuration").node().text().get();
    auto from_node = sc.node().child("From");
    auto to_node = sc.node().child("To");

    auto stop_place_idx_from =
        from_node.child("StopPlaceRef").attribute("ref").value();
    auto quay_idx_from = from_node.child("QuayRef").attribute("ref").value();
    auto stop_place_idx_to =
        to_node.child("StopPlaceRef").attribute("ref").value();
    auto quay_idx_to = to_node.child("QuayRef").attribute("ref").value();

    connection_map.try_emplace(
        id, site_connection{id, walk_transfer_duration, stop_place_idx_from,
                            quay_idx_from, stop_place_idx_to, quay_idx_to});

    // Todo somehow register these connections in the timetable, no?
    std::cout << tt.n_locations()
              << std::endl;  // This is just so compiler does not cry
  }
}
}  // namespace nigiri::loader::netex