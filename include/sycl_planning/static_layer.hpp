#pragma once

#include <cstddef>
#include <sycl_planning/layer.hpp>

namespace sycl_planning {

template <typename CellT, template <typename> typename StorageT>
class StaticLayer : public Layer {
 public:
  StaticLayer(std::size_t dim_x, std::size_t dim_y, std::size_t dim_z,
              std::size_t resolution);

 private:
  StorageT<CellT> storage_;
};

template <typename CellT, template <typename> typename StorageT>
StaticLayer<CellT, StorageT>::StaticLayer(std::size_t dim_x, std::size_t dim_y,
                                          std::size_t dim_z,
                                          std::size_t resolution)
    : Layer{resolution}, storage_{dim_x, dim_y, dim_z, resolution} {}

}  // namespace sycl_planning
