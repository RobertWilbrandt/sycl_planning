#include "sycl_planning/map.hpp"

#include <gtest/gtest.h>

class TestLayer : public sycl_planning::Layer {
 public:
  explicit TestLayer(int i) : Layer{2}, i{i} {}

  void clear(sycl::queue& q) override {}

  int i;
};

TEST(Map, AddSingleLayer) {
  sycl_planning::Map test_map;
  EXPECT_EQ(test_map.numLayers(), 0);

  test_map.addLayer<TestLayer>("test_layer", 3);

  EXPECT_EQ(test_map.numLayers(), 1);
  EXPECT_EQ(static_cast<TestLayer&>(test_map.layer(0)).i, 3);
  EXPECT_EQ(static_cast<TestLayer&>(test_map.layer("test_layer")).i, 3);
}
