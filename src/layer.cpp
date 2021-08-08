#include "sycl_planning/layer.hpp"

namespace sycl_planning {

Layer::Layer(std::size_t resolution) : resolution_{resolution} {}

Layer::~Layer() {}

std::size_t Layer::resolution() const { return resolution_; }

}  // namespace sycl_planning
