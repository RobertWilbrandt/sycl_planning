#include "sycl_planning/dense_array_storage.hpp"

#include <gtest/gtest.h>

TEST(DenseArrayStorage, Dummy) {
  sycl_planning::DenseArrayStorage<float> test_storage(10, 20, 30, 16);
}
