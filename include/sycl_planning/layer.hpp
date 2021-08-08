#pragma once

#include <cstddef>

namespace sycl_planning {

class Layer {
 public:
  explicit Layer(std::size_t resolution);
  virtual ~Layer();

  std::size_t resolution() const;

 private:
  std::size_t resolution_;
};

}  // namespace sycl_planning
