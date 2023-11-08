//
// Created by mirko on 11/1/23.
//
#include "nigiri/types.h"
#include "gtest/gtest.h"

using namespace nigiri;

namespace {
struct dummy_struct {
  int a;
  int b;
};
}  // namespace

// Used for testing the functionality of custom hash_map datastructure
TEST(netex, dummy) {
  hash_map<int, int> test_map;
  test_map.emplace(2, 5);
  auto test_item = test_map[2];
  auto test_item2 = test_map[3];

  std::cout << "DEBUGGING: " << test_item2 << std::endl;
  ASSERT_EQ(test_item, 5);
  ASSERT_EQ(test_item2, NULL);

  hash_map<int, dummy_struct> dummy_map;
  auto test_dummy = dummy_map[1];
  std::cout << "DUMMY OBJECT: " << test_dummy.a << std::endl;
}