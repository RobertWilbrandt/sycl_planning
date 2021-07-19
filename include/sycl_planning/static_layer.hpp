#pragma once

#include <cstddef>
#include <sycl_planning/layer.hpp>

namespace sycl_planning {

template <typename CellT, template <typename> typename StorageT>
class StaticLayer : public Layer {
 public:
  StaticLayer(std::size_t dim_x, std::size_t dim_y, std::size_t dim_z);

 private:
};

template <typename CellT, template <typename> typename StorageT>
StaticLayer<CellT, StorageT>::StaticLayer(std::size_t dim_x, std::size_t dim_y,
                                          std::size_t dim_z) {}

}  // namespace sycl_planning
