#include "sycl_planning/static_layer.hpp"

#include <gtest/gtest.h>

template <typename T>
class DummyStorage {};

TEST(StaticLayer, Dummy) {
  sycl_planning::StaticLayer<float, DummyStorage> layer(30, 20, 10);
}
