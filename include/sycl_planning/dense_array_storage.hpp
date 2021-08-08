#pragma once

#include <cstddef>

namespace sycl_planning {

template <typename CellT>
class DenseArrayStorage {
 public:
  DenseArrayStorage(std::size_t dim_x, std::size_t dim_y, std::size_t dim_z,
                    std::size_t resolution);

 private:
};

template <typename CellT>
DenseArrayStorage<CellT>::DenseArrayStorage(std::size_t dim_x, std::size_t y,
                                            std::size_t z,
                                            std::size_t resolution) {}

}  // namespace sycl_planning
