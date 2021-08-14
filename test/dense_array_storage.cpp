#include "sycl_planning/dense_array_storage.hpp"

#include <gtest/gtest.h>

#include "sycl_planning/cell.hpp"

TEST(DenseArrayStorage, Dummy) {
  sycl::queue q{sycl::host_selector{}};
  sycl_planning::DenseArrayStorage<sycl_planning::PrimitiveCell<float>>
      test_storage(sycl_planning::Extent3s{10, 20, 30}, 16, q);
}
