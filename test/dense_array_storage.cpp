#include "sycl_planning/dense_array_storage.hpp"

#include <gtest/gtest.h>

#include "sycl_planning/cell.hpp"

TEST(DenseArrayStorage, BasicReadWrite) {
  sycl::queue q{sycl::host_selector{}};
  sycl_planning::DenseArrayStorage<sycl_planning::PrimitiveCell<int>>
      test_storage{sycl_planning::Extent3s{1, 2, 3}, 2, q};

  EXPECT_EQ(test_storage.extent(), (sycl_planning::Extent3s{2, 4, 6}));

  /*
   * Create test pattern using an accessor with offset:
   *              / 0, x < 1  or y < 2  or z < 1
   * d[x, y, z] = | 2, x = 1 and y = 3 and z = 4
   *              \ 1, else
   */
  {
    auto test_access =
        test_storage.get_host_access<sycl_planning::AccessMode::OVERWRITE>(
            sycl_planning::Position3s{1, 2, 1});

    const auto offset = test_access.offset();
    EXPECT_EQ(offset, (sycl_planning::Position3s{1, 2, 1}));
    const auto extent = test_access.extent();
    EXPECT_EQ(extent, (sycl_planning::Extent3s{1, 2, 5}));

    for (size_t x = 0; x < extent.x; ++x) {
      for (size_t y = 0; y < extent.y; ++y) {
        for (size_t z = 0; z < extent.z; ++z) {
          if ((x == 0) && (y == 1) && (z == 3)) {
            test_access[sycl_planning::Position3s{x, y, z}] = 2;
          } else {
            test_access[sycl_planning::Position3s{x, y, z}] = 1;
          }
        }
      }
    }
  }

  /*
   * Verify that we actually created the pattern
   */
  {
    auto access =
        test_storage.get_host_access<sycl_planning::AccessMode::READ>();

    EXPECT_EQ(access.offset(), sycl_planning::Position3s::origin);
    const auto extent = access.extent();
    EXPECT_EQ(extent, test_storage.extent());

    for (std::size_t x = 0; x < extent.x; ++x) {
      for (std::size_t y = 0; y < extent.y; ++y) {
        for (std::size_t z = 0; z < extent.z; ++z) {
          const auto pos = sycl_planning::Position3s{x, y, z};
          if ((x < 1) || (y < 2) || (z < 1)) {
            EXPECT_EQ(access[pos], 0) << "At " << pos;
          } else if ((x == 1) && (y == 3) && (z == 4)) {
            EXPECT_EQ(access[pos], 2) << "At " << pos;
          } else {
            EXPECT_EQ(access[pos], 1) << "At " << pos;
          }
        }
      }
    }
  }

  /*
   * Manipulate data in window (0, 1, 0) - (1, 2, 5):
   * d[x, y, z] = 3 * d[x, y, z] + 1
   */
  q.submit([&](sycl::handler& cgh) {
    auto access =
        test_storage.get_access<sycl_planning::AccessMode::READ_WRITE>(
            cgh, sycl_planning::Position3s{0, 1, 0},
            sycl_planning::Extent3s{2, 2, 6});

    const auto extent = access.extent();
    const auto range = sycl::range<3>{extent.x, extent.y, extent.z};
    cgh.parallel_for<class ManipulateData>(range, [=](sycl::id<3> id) {
      auto& data =
          access[sycl_planning::Position3s{id.get(0), id.get(1), id.get(2)}];
      data = 3 * data + 1;
    });
  });

  /*
   * Verify expected result:
   *              / 0, y < 1                      // No data manipulation
   * d[x, y, z] = | 1, y = 1                      // Originally 0
   *              | 1, y = 2 and (x < 1 or z < 1) // Originally 0
   *              | 4, y = 2 and x = 1 and z > 0  // Originally 1
   *              | 2, x = 1 and y = 3 and z = 4  // No data manipulation
   *              | 0, y = 3 and (x < 1 or z < 1) // No data manipulation
   *              \ 1, else                       // No data manipulation
   */
  {
    auto test_access =
        test_storage.get_host_access<sycl_planning::AccessMode::READ>();

    const auto offset = test_access.offset();
    EXPECT_EQ(offset, sycl_planning::Position3s::origin);
    const auto extent = test_access.extent();
    EXPECT_EQ(extent, test_storage.extent());

    for (size_t x = 0; x < extent.x; ++x) {
      for (size_t y = 0; y < extent.y; ++y) {
        for (size_t z = 0; z < extent.z; ++z) {
          const auto pos = sycl_planning::Position3s{x, y, z};
          if (y == 0) {
            EXPECT_EQ((test_access[pos]), 0) << "At " << pos;
          } else if (y == 1) {
            EXPECT_EQ((test_access[pos]), 1) << "At " << pos;
          } else if ((y == 2) && ((x < 1) || (z < 1))) {
            EXPECT_EQ((test_access[pos]), 1) << "At " << pos;
          } else if (y == 2) {
            EXPECT_EQ((test_access[pos]), 4) << "At " << pos;
          } else if ((x == 1) && (y == 3) && (z == 4)) {
            EXPECT_EQ((test_access[pos]), 2) << "At " << pos;
          } else if ((y == 3) && ((x < 1) || (z < 1))) {
            EXPECT_EQ((test_access[pos]), 0) << "At " << pos;
          } else {
            EXPECT_EQ((test_access[pos]), 1) << "At " << pos;
          }
        }
      }
    }
  }

  /*
   * Check that we can clear the data
   */
  test_storage.clear(q);
  {
    auto test_access =
        test_storage.get_host_access<sycl_planning::AccessMode::READ>();

    const auto offset = test_access.offset();
    EXPECT_EQ(offset, sycl_planning::Position3s::origin);
    const auto extent = test_access.extent();
    EXPECT_EQ(extent, test_storage.extent());

    for (size_t x = 0; x < extent.x; ++x) {
      for (size_t y = 0; y < extent.y; ++y) {
        for (size_t z = 0; z < extent.z; ++z) {
          EXPECT_EQ((test_access[sycl_planning::Position3s{x, y, z}]), 0);
        }
      }
    }
  }
}
