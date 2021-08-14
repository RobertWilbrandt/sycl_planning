#include "sycl_planning/dense_array_storage.hpp"

#include <gtest/gtest.h>

TEST(DenseArrayStorage, Dummy) {
  sycl::queue q{sycl::host_selector{}};
  sycl_planning::DenseArrayStorage<float> test_storage(
      sycl_planning::Extent3s{10, 20, 30}, 16, q);
}
