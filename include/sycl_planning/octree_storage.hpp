#pragma once

#include <CL/sycl.hpp>
#include <cstddef>

#include "sycl_planning/geometry.hpp"
#include "sycl_planning/storage_access.hpp"

namespace sycl_planning {

template <typename CellT>
class OctreeStorage {
 public:
  OctreeStorage(Extent3s extent, std::size_t resolution, sycl::queue& q);

  Extent3s extent() const;

  void clear(sycl::queue& q);

  template <AccessMode ModeT,
            sycl::access::target TargetT = sycl::access::target::global_buffer>
  class Accessor {
   public:
    using ContentType = typename CellT::Content;
    static_assert(!AccessModeInfo<ModeT>::allows_write,
                  "Cannot write to octree storage from kernel code as this "
                  "would require dynamically changing the tree structure");
    using AccessType = const ContentType&;

   private:
  };

  template <AccessMode ModeT>
  class Accessor<ModeT, sycl::access::target::host_buffer> {
   public:
    using ContentType = typename CellT::Content;
    using AccessType = std::conditional_t<AccessModeInfo<ModeT>::allows_write,
                                          ContentType&, const ContentType&>;

   private:
  };

  template <AccessMode ModeT>
  using HostAccessor = Accessor<ModeT, sycl::access::target::global_buffer>;

  template <AccessMode ModeT>
  HostAccessor<ModeT> get_host_access(
      const Position3s& offset = Position3s::origin,
      const Extent3s& range = Extent3s::unbounded);
  template <AccessMode ModeT>
  Accessor<ModeT> get_access(sycl::handler& cgh,
                             const Position3s& offset = Position3s::origin,
                             const Extent3s& range = Extent3s::unbounded);

 private:
};

template <typename CellT>
OctreeStorage<CellT>::OctreeStorage(Extent3s extent, std::size_t resolution,
                                    sycl::queue& q) {}

template <typename CellT>
template <AccessMode ModeT>
typename OctreeStorage<CellT>::template HostAccessor<ModeT>
OctreeStorage<CellT>::get_host_access(const Position3s& offset,
                                      const Extent3s& range) {
  return OctreeStorage<CellT>::HostAccessor<ModeT>{};
}

template <typename CellT>
template <AccessMode ModeT>
typename OctreeStorage<CellT>::template Accessor<ModeT>
OctreeStorage<CellT>::get_access(sycl::handler& cgh, const Position3s& offset,
                                 const Extent3s& range) {
  return OctreeStorage<CellT>::Accessor<ModeT>{};
}

}  // namespace sycl_planning
