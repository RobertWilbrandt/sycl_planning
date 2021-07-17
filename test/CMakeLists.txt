find_package(GTest REQUIRED)

add_executable(basic_test
  map.cpp
)

target_link_libraries(basic_test
  PRIVATE
    sycl_planning
    GTest::GTest
    GTest::Main
)

add_test(
  NAME
    basic_test
  COMMAND
    basic_test
)
