#pragma once

#include <CL/sycl.hpp>
#include <cstddef>
#include <sycl_planning/layer.hpp>

namespace sycl_planning {

template <typename CellT, template <typename> typename StorageT>
class StaticLayer : public Layer {
 public:
  StaticLayer(std::size_t dim_x, std::size_t dim_y, std::size_t dim_z,
              std::size_t resolution, sycl::queue& q);

  void clear(sycl::queue& q) override;

 private:
  StorageT<CellT> storage_;
};

/*
 * Template definitions
 */

template <typename CellT, template <typename> typename StorageT>
StaticLayer<CellT, StorageT>::StaticLayer(std::size_t dim_x, std::size_t dim_y,
                                          std::size_t dim_z,
                                          std::size_t resolution,
                                          sycl::queue& q)
    : Layer{resolution}, storage_{dim_x, dim_y, dim_z, resolution, q} {}

template <typename CellT, template <typename> typename StorageT>
void StaticLayer<CellT, StorageT>::clear(sycl::queue& q) {
  storage_.clear(q);
}

}  // namespace sycl_planning
