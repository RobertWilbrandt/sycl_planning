#pragma once

#include <CL/sycl.hpp>
#include <cstddef>

namespace sycl_planning {

template <typename CellT>
class DenseArrayStorage {
 public:
  DenseArrayStorage(std::size_t dim_x, std::size_t dim_y, std::size_t dim_z,
                    std::size_t resolution);

 private:
  sycl::buffer<CellT, 3> buffer_;
};

template <typename CellT>
DenseArrayStorage<CellT>::DenseArrayStorage(std::size_t dim_x,
                                            std::size_t dim_y,
                                            std::size_t dim_z,
                                            std::size_t resolution)
    : buffer_{sycl::range<3>{dim_x * resolution, dim_y * resolution,
                             dim_z * resolution}} {}

}  // namespace sycl_planning
