#include "sycl_planning/octree_storage.hpp"

#include <gtest/gtest.h>

#include "sycl_planning/cell.hpp"

TEST(OctreeStorage, Dummy) {
  sycl::queue q{sycl::host_selector{}};
  sycl_planning::OctreeStorage<sycl_planning::PrimitiveCell<int>> test_storage{
      sycl_planning::Extent3s{1, 2, 3}, 2, q};

  auto host_access_read =
      test_storage.get_host_access<sycl_planning::AccessMode::READ>();
}
