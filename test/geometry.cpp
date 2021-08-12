#include "sycl_planning/geometry.hpp"

#include <gtest/gtest.h>

TEST(Geometry, TranslationBasicOperations) {
  EXPECT_EQ((sycl_planning::Translation3s::unitX +
             std::size_t{2} * sycl_planning::Translation3s::unitY +
             sycl_planning::Translation3s::unitZ * std::size_t{3}),
            (sycl_planning::Translation3s{1, 2, 3}));

  sycl_planning::Translation3i test{1, 2, 3};
  EXPECT_EQ((test + sycl_planning::Translation3i{4, 0, 1}),
            (sycl_planning::Translation3i{5, 2, 4}));

  test += sycl_planning::Translation3i{1, 2, 3};
  EXPECT_EQ(test, (sycl_planning::Translation3i{2, 4, 6}));

  test /= 2;
  EXPECT_EQ(test, (sycl_planning::Translation3i{1, 2, 3}));
  EXPECT_EQ((test * 3), (sycl_planning::Translation3i{3, 6, 9}));
}

TEST(Geometry, TranslationPositionInterop) {
  sycl_planning::Position3i test_pos{1, 2, 3};
  EXPECT_EQ((test_pos - sycl_planning::Position3i::origin),
            (sycl_planning::Translation3i{1, 2, 3}));

  test_pos += sycl_planning::Translation3i::unitZ;
  EXPECT_EQ(test_pos, (sycl_planning::Position3i{1, 2, 4}));
}

TEST(Geometry, RotationBasicOperation) {
  sycl_planning::Translation3d test_t = sycl_planning::Translation3d::unitX;
  EXPECT_NEAR(((sycl_planning::Rotation3d::aroundX(M_PI / 2) * test_t -
                sycl_planning::Translation3d::unitX)
                   .normSquared()),
              0., 0.001);
  EXPECT_NEAR(((sycl_planning::Rotation3d::aroundY(M_PI / 2) * test_t -
                -sycl_planning::Translation3d::unitZ)
                   .normSquared()),
              0., 0.001);
  EXPECT_NEAR(((sycl_planning::Rotation3d::aroundZ(M_PI / 2) * test_t -
                sycl_planning::Translation3d::unitY)
                   .normSquared()),
              0., 0.001);
}

TEST(Geometry, BasicTransform) {
  sycl_planning::Transform3d test_t{
      sycl_planning::Translation3d{1, 2, 3},
      sycl_planning::Rotation3d::aroundX(M_PI / 2)};

  EXPECT_NEAR(
      ((test_t * sycl_planning::Pose3d{sycl_planning::Position3d{1, 0, 0},
                                       sycl_planning::Orientation3d{}})
           .position -
       sycl_planning::Position3d{2, 2, 3})
          .normSquared(),
      0., 0.001);
  EXPECT_NEAR(
      ((test_t * sycl_planning::Pose3d{sycl_planning::Position3d{0, 1, 0},
                                       sycl_planning::Orientation3d{}})
           .position -
       sycl_planning::Position3d{1, 2, 4})
          .normSquared(),
      0., 0.001);
}
