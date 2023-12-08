#pragma once

#include <iosfwd>
#include <span>

#include "geo/latlng.h"

#include "nigiri/common/it_range.h"
#include "nigiri/types.h"

namespace nigiri {

struct timetable;

struct location {
  friend std::ostream& operator<<(std::ostream&, location const&);
  location(timetable const&, location_idx_t);
  location(std::string_view id,
           std::string_view name,
           geo::latlng pos,
           source_idx_t, // Wichtig, wenn wir verschiedene Datensätze(=Source_idx)
           // oder sogar mehrere Fahrpläne(da die IDs nur eindeutig
           // sind zusammen mit Source_Idx) mergen.
           location_type,  // Quays=kTrack, alles andere kStation
           location_idx_t parent, // Für Quays wahrscheinlich der StopPonit(oder SSP)
           timezone_idx_t,
           duration_t transfer_time, // Minimale Umstiegszeit, die wir bei einer Location
           // mindestens einhalten müssen (relevant für logische
           // Locations, also "Darmstadt Hauptbahnhof", für Quays
           // kann ich das auf 0 setzen)
           it_range<vector<location_idx_t>::const_iterator> equivalences); // "Äuivalente Stops fürs Routing". Also zwei
  // StpoPoints könnten als äquivalent gelten. Also
  // könnte ich potentiell auch das nutzen statt den Baum
  // zu flattenen
  location_idx_t l_{location_idx_t::invalid()};
  std::string_view id_;
  std::string_view name_;
  geo::latlng pos_;
  source_idx_t src_;
  location_type type_;
  location_idx_t parent_;
  timezone_idx_t timezone_idx_;
  duration_t transfer_time_;
  it_range<vector<location_idx_t>::const_iterator> equivalences_;
};

}  // namespace nigiri
