#include "gtest/gtest.h"

#include "nigiri/loader/netex/stop_assignment.h"

using namespace nigiri;
using namespace nigiri::loader::netex;

/* Goal for testing:
 *  1. We correctly build up a map for the assignments
 *  2. We construct locations for each scheduled_stop_point
 *  3. We construct locations for each stop_place->quay
 *  4. The quays and the ssp are linked correctly via the parent reference
 *  5. Faulty & edge cases
 */

constexpr auto const netex_input =
    R"(<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<PublicationDelivery xmlns="http://www.netex.org.uk/netex" version="ntx:1.1">
    <dataObjects>
        <CompositeFrame id="DE::CompositeFrame_EU_PI_LINE_OFFER:144-SILBUS-V95" version="1641291376">
            <ValidBetween>
                <FromDate>2021-12-21T00:00:00</FromDate>
                <ToDate>2022-06-11T00:00:00</ToDate>
            </ValidBetween>
            <frames>
                <ServiceFrame id="DE::ServiceFrame_EU_PI_NETWORK:144-SILBUS-V95" version="1641291376">
                    <TypeOfFrameRef ref="epip:EU_PI_NETWORK" versionRef="1.0"/>
                    <stopAssignments>
                        <PassengerStopAssignment id="Stop_Assignment_0" order="1"
                                                 version="1641291376">
                            <ScheduledStopPointRef ref="SSP_0" version="1641291376"/>
                            <StopPlaceRef ref="SP_2" version="1641291376"/>
                        </PassengerStopAssignment>
                        <PassengerStopAssignment id="Stop_Assignment_1" order="1"
                                                 version="1641291376">
                            <ScheduledStopPointRef ref="SSP_1" version="1641291376"/>
                            <StopPlaceRef ref="SP_1" version="1641291376"/>
                        </PassengerStopAssignment>
                        <PassengerStopAssignment id="Stop_Assignment_2" order="1"
                                                 version="1641291376">
                            <ScheduledStopPointRef ref="SSP_2" version="1641291376"/>
                            <StopPlaceRef ref="SP_0" version="1641291376"/>
                        </PassengerStopAssignment>
                    </stopAssignments>
                </ServiceFrame>
            </frames>
        </CompositeFrame>
    </dataObjects>
</PublicationDelivery>)";

void initialize_maps(
    hash_map<std::string_view, stop_place>& stop_map,
    hash_map<std::string_view, scheduled_stop_point>& ssp_map) {
  hash_map<std::string_view, quay> quays0;
  quay quay0{"q0", "q0", geo::latlng()};
  quays0.try_emplace("0", quay0);

  hash_map<std::string_view, quay> quays1;
  quay quay1{"q1", "q1", geo::latlng()};
  quays1.try_emplace("1", quay1);

  hash_map<std::string_view, quay> quays2;
  quay quay2{"q2", "q2", geo::latlng()};
  quays2.try_emplace("2", quay2);

  stop_map.try_emplace("SP_0",
                       stop_place{"SP_0", "StopPlace0", geo::latlng(), quays0});
  stop_map.try_emplace("SP_1",
                       stop_place{"SP_1", "StopPlace1", geo::latlng(), quays1});
  stop_map.try_emplace("SP_2",
                       stop_place{"SP_2", "StopPlace2", geo::latlng(), quays2});

  ssp_map.try_emplace("SSP_0",
                      scheduled_stop_point{"SSP_0", "Test0", "busStation"});
  ssp_map.try_emplace("SSP_1",
                      scheduled_stop_point{"SSP_1", "Test1", "busStation"});
  ssp_map.try_emplace("SSP_2",
                      scheduled_stop_point{"SSP_2", "Test2", "busStation"});
}

TEST(nigiri, stop_assignment) {
  // Step 1: Get our input xml
  auto doc = pugi::xml_document{};
  auto const result = doc.load_string(netex_input);
  ASSERT_TRUE(result);

  // Step 2: Call read_stop_assignments
  timetable tt;
  hash_map<std::string_view, stop_assignment> stop_assignment_map;
  hash_map<std::string_view, stop_place> stop_map;
  hash_map<std::string_view, scheduled_stop_point> ssp_map;
  initialize_maps(stop_map, ssp_map);

  read_stop_assignments(doc, stop_assignment_map, stop_map, ssp_map, tt);
  ASSERT_EQ(stop_assignment_map.size(), 3);

  auto first_stop_assignment = stop_assignment_map["Stop_Assignment_0"];
  ASSERT_EQ(first_stop_assignment.ssp_ref_, "SSP_0");
  ASSERT_EQ(first_stop_assignment.stop_place_ref_, "SP_2");
}

TEST(nigiri, stop_assignment_ssp_locations) {
  // Step 1: Get our input xml
  auto doc = pugi::xml_document{};
  auto const result = doc.load_string(netex_input);
  ASSERT_TRUE(result);

  // Step 2: Call read_stop_assignments
  timetable tt;
  hash_map<std::string_view, stop_assignment> stop_assignment_map;
  hash_map<std::string_view, stop_place> stop_map;
  hash_map<std::string_view, scheduled_stop_point> ssp_map;
  initialize_maps(stop_map, ssp_map);

  read_stop_assignments(doc, stop_assignment_map, stop_map, ssp_map, tt);

  // Step 3: Check whether the locations have been created correctly out of the
  // ssps and quays
  auto locs = tt.locations_;
  ASSERT_EQ(locs.names_.size(),
            6);  // Should have registered 3 SSPs and 3 Stop Points

  auto first_ssp_location = locs.get(location_idx_t(0));
  ASSERT_EQ(first_ssp_location.id_, "SSP_0");
  ASSERT_EQ(first_ssp_location.parent_, location_idx_t::invalid());
  ASSERT_EQ(first_ssp_location.type_, location_type::kStation);

  auto first_sp_location = locs.get(location_idx_t(1));
  ASSERT_EQ(first_sp_location.id_, "q2");
  ASSERT_EQ(first_sp_location.parent_, location_idx_t(0));
  ASSERT_EQ(first_sp_location.type_, location_type::kTrack);
}
