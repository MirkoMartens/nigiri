#include "nigiri/loader/netex/stop_assignment.h"

namespace nigiri::loader::netex {

void read_stop_assignments(
    const pugi::xml_document& doc,
    source_idx_t& src,
    hash_map<std::string_view, stop_assignment>& stop_assignment_map,
    hash_map<std::string_view, stop_place>& stop_map,
    hash_map<std::string_view, scheduled_stop_point>& ssp_map,
    timetable& tt) {
  for (auto const& sa : doc.select_nodes("//PassengerStopAssignment")) {
    auto const id = sa.node().attribute("id").value();

    auto ssp_ref =
        sa.node().child("ScheduledStopPointRef").attribute("ref").value();
    auto sp_ref = sa.node().child("StopPlaceRef").attribute("ref").value();
    stop_assignment_map.try_emplace(id, stop_assignment{id, ssp_ref, sp_ref});

    // Filling locations
    auto stop = stop_map.at(sp_ref);
    auto ssp = ssp_map.at(ssp_ref);

    // 1. Create location for ssp/stop point
    auto empty_idx_vec = vector<location_idx_t>{};
    location l{ssp.id_,
               ssp.name_,
               stop.coords_,
               src,
               location_type::kStation,
               location_idx_t::invalid(),
               timezone_idx_t::invalid(),
               10_minutes,
               it_range{empty_idx_vec}};
    auto ssp_idx = tt.locations_.register_location({l});

    // 2. Create a location object for every quay and use hierarchical structure
    // TODO would currenty fail if quays are added several times!
    // Solution: HAVING MY OWN LOCATIONS MAP TO CHECK FOR DUPLICATES!!!
    for (auto const& quay_pair : stop.quays_) {
      auto quay = quay_pair.second;
      location quay_loc(quay.id_, quay.name_, quay.coords_, src,
                        location_type::kTrack, ssp_idx,
                        timezone_idx_t::invalid(), 0_minutes,
                        it_range{empty_idx_vec});
      tt.locations_.register_location(quay_loc);
    }
  }
}
}  // namespace nigiri::loader::netex