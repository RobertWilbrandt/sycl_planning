cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_COMPILER dpcpp)

project(sycl_planning)

add_library(sycl_planning
  src/map.cpp
)

target_include_directories(sycl_planning
  PUBLIC
    include
)

target_link_libraries(${PROJECT_NAME}
  sycl
)

if(${BUILD_TESTS})
  enable_testing()
  add_subdirectory(test)
endif()
