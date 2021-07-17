#include "sycl_planning/map.hpp"

#include <CL/sycl.hpp>
#include <iostream>

namespace sycl_planning {

void print_sycl_dev() {
  sycl::queue q{sycl::default_selector{}};
  auto device = q.get_device();
  std::cout << "Platform: "
            << device.get_platform().get_info<sycl::info::platform::name>()
            << "\nDevice: " << device.get_info<sycl::info::device::name>()
            << std::endl;
}

}  // namespace sycl_planning
