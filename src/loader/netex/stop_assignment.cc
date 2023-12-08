#include "nigiri/loader/netex/stop_assignment.h"

namespace nigiri::loader::netex {

void read_stop_assignments(
    const pugi::xml_document& doc,
    hash_map<std::string_view, stop_assignment> stop_assignment_map) {
  for (auto const& sa : doc.select_nodes("//PassengerStopAssignment")) {
    auto const id = sa.node().attribute("id").value();

    auto ssp_ref =
        sa.node().child("ScheduledStopPointRef").attribute("ref").value();
    auto sp_ref = sa.node().child("StopPlaceRef").attribute("ref").value();

    // TODO HERE: POTENTIALLY MERGE INTO ONE LOCATION STRUCT?
    stop_assignment_map.try_emplace(id, stop_assignment{id, ssp_ref, sp_ref});
  }
}
}  // namespace nigiri::loader::netex