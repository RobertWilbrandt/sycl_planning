#pragma once

#include <CL/sycl.hpp>

namespace sycl_planning {

enum class AccessMode { READ, WRITE, READ_WRITE, OVERWRITE };

template <AccessMode ModeT>
struct AccessModeInfo {};

template <>
struct AccessModeInfo<AccessMode::READ> {
  static constexpr sycl::access::mode sycl_value = sycl::access::mode::read;
  static constexpr bool allows_write = false;
};

template <>
struct AccessModeInfo<AccessMode::WRITE> {
  static constexpr sycl::access::mode sycl_value = sycl::access::mode::write;
  static constexpr bool allows_write = true;
};

template <>
struct AccessModeInfo<AccessMode::READ_WRITE> {
  static constexpr sycl::access::mode sycl_value =
      sycl::access::mode::read_write;
  static constexpr bool allows_write = true;
};

template <>
struct AccessModeInfo<AccessMode::OVERWRITE> {
  static constexpr sycl::access::mode sycl_value =
      sycl::access::mode::discard_write;
  static constexpr bool allows_write = true;
};

}  // namespace sycl_planning
