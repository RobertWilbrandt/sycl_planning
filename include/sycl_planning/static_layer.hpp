#pragma once

#include <CL/sycl.hpp>
#include <cstddef>

#include "sycl_planning/geometry.hpp"
#include "sycl_planning/layer.hpp"

namespace sycl_planning {

template <typename CellT, template <typename> typename StorageT>
class StaticLayer : public Layer {
 public:
  StaticLayer(Extents extent, std::size_t resolution, sycl::queue& q);

  void clear(sycl::queue& q) override;

 private:
  StorageT<CellT> storage_;
};

/*
 * Template definitions
 */

template <typename CellT, template <typename> typename StorageT>
StaticLayer<CellT, StorageT>::StaticLayer(Extents extent,
                                          std::size_t resolution,
                                          sycl::queue& q)
    : Layer{resolution}, storage_{extent, resolution, q} {}

template <typename CellT, template <typename> typename StorageT>
void StaticLayer<CellT, StorageT>::clear(sycl::queue& q) {
  storage_.clear(q);
}

}  // namespace sycl_planning
