cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_COMPILER dpcpp)

project(sycl_planning)

add_library(sycl_planning
  src/map.cpp
  src/layer.cpp
)

target_include_directories(sycl_planning
  PUBLIC
    include
)

target_link_libraries(${PROJECT_NAME}
  sycl
)

option(BUILD_DOC "Build documentation")
if (${BUILD_DOC})
  add_subdirectory(doc)
endif ()

option(BUILD_TESTS "Build project tests")
if(${BUILD_TESTS})
  enable_testing()
  add_subdirectory(test)
endif()
