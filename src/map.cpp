#include "sycl_planning/map.hpp"

#include <CL/sycl.hpp>
#include <algorithm>
#include <iostream>
#include <stdexcept>

namespace sycl_planning {

Map::Map() {}

size_t Map::numLayers() const { return layers_.size(); }

Layer& Map::layer(size_t i) { return *layers_[i].second; }

const Layer& Map::layer(size_t i) const { return *layers_[i].second; }

Layer& Map::layer(const std::string& name) {
  auto it = std::find_if(
      layers_.begin(), layers_.end(),
      [&name](const std::pair<std::string, std::unique_ptr<Layer>>& p) {
        return name == p.first;
      });

  if (it != layers_.end()) {
    return *it->second;
  } else {
    throw std::runtime_error{std::string("No layer with name ") + name};
  }
}

const Layer& Map::layer(const std::string& name) const {
  auto it = std::find_if(
      layers_.begin(), layers_.end(),
      [&name](const std::pair<std::string, std::unique_ptr<Layer>>& p) {
        return name == p.first;
      });

  if (it != layers_.end()) {
    return *it->second;
  } else {
    throw std::runtime_error{std::string("No layer with name ") + name};
  }
}

}  // namespace sycl_planning
