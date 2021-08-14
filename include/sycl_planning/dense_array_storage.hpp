#pragma once

#include <CL/sycl.hpp>
#include <cstddef>

#include "sycl_planning/geometry.hpp"

namespace sycl_planning {

template <typename CellT>
class DenseArrayStorage {
 public:
  DenseArrayStorage(Extent3s extent, std::size_t resolution, sycl::queue& q);

  void clear(sycl::queue& q);

 private:
  sycl::buffer<CellT, 3> buffer_;
};

/*
 * Template definitions
 */

template <typename CellT>
DenseArrayStorage<CellT>::DenseArrayStorage(Extent3s extent,
                                            std::size_t resolution,
                                            sycl::queue& q)
    : buffer_{sycl::range<3>{extent.x * resolution, extent.y * resolution,
                             extent.z * resolution}} {
  clear(q);
}

template <typename CellT>
void DenseArrayStorage<CellT>::clear(sycl::queue& q) {}

}  // namespace sycl_planning
