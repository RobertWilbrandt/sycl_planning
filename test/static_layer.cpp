#include "sycl_planning/static_layer.hpp"

#include <gtest/gtest.h>

template <typename T>
class DummyStorage {
 public:
  DummyStorage(std::size_t dim_x, std::size_t dim_y, std::size_t dim_z,
               std::size_t resolution, sycl::queue& q) {}

  void clear(sycl::queue& q) {}
};

TEST(StaticLayer, Dummy) {
  sycl::queue q{sycl::host_selector{}};
  sycl_planning::StaticLayer<float, DummyStorage> layer(30, 20, 10, 8, q);
}
