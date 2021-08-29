#pragma once

namespace sycl_planning {

template <typename T>
class PrimitiveCell {
 public:
  using Content = T;

  static void clear(Content& c);
};

/*
 * Template definitions
 */

template <typename T>
void PrimitiveCell<T>::clear(Content& c) {
  c = T{0};
}

}  // namespace sycl_planning
