#pragma once

#include <CL/sycl.hpp>

namespace sycl_planning {

enum class AccessMode { READ, WRITE, READ_WRITE, OVERWRITE };

template <AccessMode ModeT>
struct SyclAccessMode {};

template <>
struct SyclAccessMode<AccessMode::READ> {
  static constexpr sycl::access::mode value = sycl::access::mode::read;
};

template <>
struct SyclAccessMode<AccessMode::WRITE> {
  static constexpr sycl::access::mode value = sycl::access::mode::write;
};

template <>
struct SyclAccessMode<AccessMode::READ_WRITE> {
  static constexpr sycl::access::mode value = sycl::access::mode::read_write;
};

template <>
struct SyclAccessMode<AccessMode::OVERWRITE> {
  static constexpr sycl::access::mode value = sycl::access::mode::discard_write;
};

}  // namespace sycl_planning
