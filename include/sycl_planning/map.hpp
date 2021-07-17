#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "sycl_planning/layer.hpp"

namespace sycl_planning {

class Map {
 public:
  Map();

  template <typename LayerT, typename... ArgsTs>
  void addLayer(const std::string& name, ArgsTs&&... args);

  size_t numLayers() const;

  Layer& layer(size_t i);
  const Layer& layer(size_t i) const;
  Layer& layer(const std::string& name);
  const Layer& layer(const std::string& name) const;

 private:
  std::vector<std::pair<std::string, std::unique_ptr<Layer>>> layers_;
};

/*
 * Template definitions
 */

template <typename LayerT, typename... ArgsTs>
void Map::addLayer(const std::string& name, ArgsTs&&... args) {
  layers_.emplace_back(std::make_pair(
      name, std::make_unique<LayerT>(std::forward<ArgsTs>(args)...)));
}

}  // namespace sycl_planning
