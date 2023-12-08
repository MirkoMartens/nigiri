#include "nigiri/loader/netex/load_timetable.h"

#include "nigiri/loader/dir.h"
#include "nigiri/timetable.h"
#include "nigiri/types.h"

#include "nigiri/loader/netex/route_operator.h"
#include "nigiri/loader/netex/service_calendar.h"
#include "nigiri/loader/netex/service_link.h"
#include "nigiri/loader/netex/stop_assignment.h"
#include "nigiri/loader/netex/stop_place.h"

#include "nigiri/loader/netex/line.h"
#include "nigiri/loader/netex/scheduled_stop_point.h"
#include "nigiri/loader/netex/service_journey_pattern.h"
#include "nigiri/loader/netex/site_connection.h"

#include "pugixml.hpp"

namespace nigiri::loader::netex::utils {

struct netex_timetable {
  gtfs::tz_map timezones;
  hash_map<std::string_view, provider_idx_t> operatorMap;
  hash_map<std::string_view, bitfield> calendar;
  hash_map<std::string_view, bitfield> operating_periods;
  hash_map<std::string_view, stop_place> stop_places;
  hash_map<std::string_view, line> line_map;
  hash_map<std::string_view, scheduled_stop_point> stops_map;
  hash_map<std::string_view, service_link> service_links_map;
  hash_map<std::string_view, site_connection> connection_map;
  hash_map<std::string_view, stop_assignment> stop_assignment_map;
  hash_map<std::string_view, service_journey_pattern> journey_patterns;
};

bool isDirectory(const char* filename) {
  size_t length = std::strlen(filename);
  return filename[length - 1] == '/';
}

// Entry point - this method is called for every single xml file, and we
// gradually build a timetable object
void handle_xml_parse_result(timetable& tt,
                             pugi::xml_document& doc,
                             netex_timetable& netex_tt) {

  // 1. ResourceFrame -> Defines responsibilities, authorities etc
  read_resource_frame(tt, doc, netex_tt.operatorMap);
  std::cout << netex_tt.timezones.size();  // todo delete

  // 2. ServiceCalendarFrame -> Defines the days when a connection is valid
  read_service_calendar(tt, doc, netex_tt.calendar, netex_tt.operating_periods);

  // 3. SiteFrame -> Defines the physical stops and quays
  read_stop_places(doc, netex_tt.stop_places);

  // 4. ServiceFrame -> Contains all the logic about stops, routes, journeys
  read_lines(tt, doc, netex_tt.line_map, netex_tt.operatorMap);
  read_scheduled_stop_points(doc, netex_tt.stops_map);
  read_service_links(doc, netex_tt.service_links_map);
  read_site_connections(tt, doc, netex_tt.connection_map);
  read_stop_assignments(doc, netex_tt.stop_assignment_map, netex_tt.stop_places,
                        netex_tt.stops_map, tt);
  read_journey_patterns(doc, netex_tt.journey_patterns, netex_tt.line_map,
                        netex_tt.stops_map);

  // 5. TimetableFrame, contains the exact timetable info about journeys
  // read_service_journeys(doc, netex_tt.service_journeys);
  // auto const timetableFrame = frames.child("TimetableFrame");
}
}  // namespace nigiri::loader::netex::utils

/*
 * Entry point that is called by load_timetable
 */
namespace fs = std::filesystem;
namespace utils = nigiri::loader::netex::utils;

namespace nigiri::loader::netex {

void load_timetable(source_idx_t src, dir const& d, timetable& t) {

  // Step 0: Define all the variables needed throughout the parsing
  gtfs::tz_map timezones{};

  hash_map<std::string_view, provider_idx_t> operatorMap{};
  hash_map<std::string_view, bitfield> calendar{};
  hash_map<std::string_view, bitfield> operating_periods{};
  hash_map<std::string_view, stop_place> stop_places{};
  hash_map<std::string_view, line> line_map{};
  hash_map<std::string_view, scheduled_stop_point> stops_map{};
  hash_map<std::string_view, service_link> service_links_map{};
  hash_map<std::string_view, site_connection> connection_map{};
  hash_map<std::string_view, stop_assignment> stop_assignment_map{};
  hash_map<std::string_view, service_journey_pattern> journey_patterns{};

  utils::netex_timetable netex_tt{
      timezones,           operatorMap,       calendar,
      operating_periods,   stop_places,       line_map,
      stops_map,           service_links_map, connection_map,
      stop_assignment_map, journey_patterns};

  // Step 1: Load the zip file
  auto vecOfPaths = d.list_files("");
  // Step 2: Process all files iteratively
  for (auto const& path : vecOfPaths) {
    auto file = d.get_file(path);

    // Break if the filename is just the directory
    if (utils::isDirectory(file.filename())) {
      continue;
    }

    // Step 3: Parse the xml document into the pugi data structure
    pugi::xml_document doc;
    // Here, we need to convert the xml data to a pugi::char_t* string
    std::basic_string<pugi::char_t> convertedData(file.data().begin(),
                                                  file.data().end());
    auto const result = doc.load_string(convertedData.c_str());

    log(log_lvl::info, "loader.netex.load_timetable",
        R"(Status of parsed xml: "{}"for file: "{}")", result.status,
        file.filename());

    // Step 4: Handle the parse result and build up the data structures
    utils::handle_xml_parse_result(t, doc, netex_tt);

    // Step 5: Process the data structures in the same way as
    // gtfs/load_timetable.cc from line 106 on
    std::cout << src;  // TODO DELETE
  }
}

}  // namespace nigiri::loader::netex