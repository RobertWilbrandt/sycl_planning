#include "sycl_planning/dense_array_storage.hpp"

#include <gtest/gtest.h>

#include "sycl_planning/cell.hpp"

TEST(DenseArrayStorage, BasicReadWrite) {
  sycl::queue q{sycl::host_selector{}};
  sycl_planning::DenseArrayStorage<sycl_planning::PrimitiveCell<int>>
      test_storage{sycl_planning::Extent3s{1, 2, 3}, 2, q};
  const auto extent = test_storage.extent();

  {
    auto test_access =
        test_storage.get_access<sycl_planning::AccessMode::OVERWRITE>();

    for (size_t x = 0; x < extent.x; ++x) {
      for (size_t y = 0; y < extent.y; ++y) {
        for (size_t z = 0; z < extent.z; ++z) {
          if ((x == 1) && (y == 3) && (z == 5)) {
            test_access[sycl_planning::Position3s{x, y, z}] = 1;
          } else {
            test_access[sycl_planning::Position3s{x, y, z}] = 0;
          }
        }
      }
    }
  }

  q.submit([&](sycl::handler& cgh) {
    auto access =
        test_storage.get_access<sycl_planning::AccessMode::READ_WRITE>(cgh);

    const auto range = sycl::range<3>{extent.x, extent.y, extent.z};
    cgh.parallel_for<class ManipulateData>(range, [=](sycl::id<3> id) {
      auto& data =
          access[sycl_planning::Position3s{id.get(0), id.get(1), id.get(2)}];
      data = 3 * data + 1;
    });
  });

  {
    auto test_access =
        test_storage.get_access<sycl_planning::AccessMode::READ>();

    for (size_t x = 0; x < extent.x; ++x) {
      for (size_t y = 0; y < extent.y; ++y) {
        for (size_t z = 0; z < extent.z; ++z) {
          if ((x == 1) && (y == 3) && (z == 5)) {
            EXPECT_EQ((test_access[sycl_planning::Position3s{x, y, z}]), 4);
          } else {
            EXPECT_EQ((test_access[sycl_planning::Position3s{x, y, z}]), 1);
          }
        }
      }
    }
  }
}
