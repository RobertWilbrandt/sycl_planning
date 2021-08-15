#include "sycl_planning/dense_array_storage.hpp"

#include <gtest/gtest.h>

#include "sycl_planning/cell.hpp"

TEST(DenseArrayStorage, Dummy) {
  sycl::queue q{sycl::host_selector{}};
  sycl_planning::DenseArrayStorage<sycl_planning::PrimitiveCell<float>>
      test_storage(sycl_planning::Extent3s{10, 20, 30}, 16, q);

  {
    auto test_access =
        test_storage.get_access<sycl_planning::AccessMode::OVERWRITE>();
  }

  {
    q.submit([&](sycl::handler& cgh) {
      auto test_access =
          test_storage.get_access<sycl_planning::AccessMode::READ>(cgh);

      const auto extent = test_storage.extent();
      cgh.parallel_for<class TestKernel>(
          sycl::range<3>{extent.x, extent.y, extent.z}, [=](sycl::id<3> id) {

          });
    });
  }
}
