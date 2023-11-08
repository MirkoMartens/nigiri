#include "gtest/gtest.h"

#include "nigiri/loader/netex/load_timetable.h"

using namespace nigiri;
using namespace nigiri::loader::netex;

TEST(netex, load_timetable_nonexistent_file) {

  timetable t;
  auto const fs = loader::fs_dir{"test/test_data/nonexistentfile"};
  try {
    load_timetable(source_idx_t{0}, fs, t);
    FAIL() << "Don't go here!" << std::endl;
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    ASSERT_TRUE(true);
  }
}

TEST(netex, load_timetable) {
  timetable t;
  auto const zip = loader::zip_dir("test/test_data/line144_silbus.zip");
  load_timetable(source_idx_t{0}, zip, t);
  // Checking a couple of routes etc.
  ASSERT_GE(t.n_locations(), 0);
}
