#pragma once

#include <CL/sycl.hpp>
#include <cstddef>

namespace sycl_planning {

class Layer {
 public:
  explicit Layer(std::size_t resolution);
  virtual ~Layer();

  std::size_t resolution() const;

  virtual void clear(sycl::queue& q) = 0;

 private:
  std::size_t resolution_;
};

}  // namespace sycl_planning
