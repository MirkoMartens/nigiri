#include "nigiri/loader/netex/service_link.h"

namespace nigiri::loader::netex {

void read_service_links(
    const pugi::xml_document& doc,
    hash_map<std::string_view, service_link>& service_links_map) {
  for (auto const& sl : doc.select_nodes("//ServiceLink")) {
    auto const id = sl.node().attribute("id").value();

    auto from_point_ref =
        sl.node().child("FromPointRef").attribute("ref").value();
    auto to_point_ref = sl.node().child("ToPointRef").attribute("ref").value();

    service_links_map.try_emplace(
        id, service_link{id, from_point_ref, to_point_ref});
  }
}

}  // namespace nigiri::loader::netex