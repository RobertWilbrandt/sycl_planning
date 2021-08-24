#pragma once

#include <CL/sycl.hpp>
#include <cstddef>
#include <type_traits>

#include "sycl_planning/geometry.hpp"
#include "sycl_planning/storage_access.hpp"

namespace sycl_planning {

template <typename CellT>
class DenseArrayStorage {
 public:
  DenseArrayStorage(Extent3s extent, std::size_t resolution, sycl::queue& q);

  Extent3s extent() const;

  void clear(sycl::queue& q);

  template <AccessMode ModeT, sycl::access::target TargetT>
  class Accessor {
   public:
    template <sycl::access::target TestTargetT = TargetT>
    explicit Accessor(typename std::enable_if<
                      TestTargetT == sycl::access::target::host_buffer,
                      sycl::buffer<typename CellT::Content, 3>&>::type buffer);
    Accessor(sycl::buffer<typename CellT::Content, 3>& buffer,
             sycl::handler& cgh);

    using ContentType = typename CellT::Content;
    using AccessType = std::conditional_t<AccessModeInfo<ModeT>::allows_write,
                                          ContentType&, const ContentType&>;

    AccessType operator[](const Position3s& p) const;

   private:
    sycl::accessor<typename CellT::Content, 3,
                   AccessModeInfo<ModeT>::sycl_value, TargetT>
        access_;
  };

  template <AccessMode ModeT>
  Accessor<ModeT, sycl::access::target::host_buffer> get_access(
      const Position3s& offset = Position3s::origin,
      const Extent3s& range = Extent3s::unbounded);
  template <AccessMode ModeT>
  Accessor<ModeT, sycl::access::target::global_buffer> get_access(
      sycl::handler& cgh, const Position3s& offset = Position3s::origin,
      const Extent3s& range = Extent3s::unbounded);

 private:
  sycl::buffer<typename CellT::Content, 3> buffer_;
};

/*
 * Template definitions
 */

template <typename CellT>
DenseArrayStorage<CellT>::DenseArrayStorage(Extent3s extent,
                                            std::size_t resolution,
                                            sycl::queue& q)
    : buffer_{sycl::range<3>{extent.x * resolution, extent.y * resolution,
                             extent.z * resolution}} {
  clear(q);
}

template <typename CellT>
Extent3s DenseArrayStorage<CellT>::extent() const {
  const auto range = buffer_.get_range();
  return Extent3s{range.get(0), range.get(1), range.get(2)};
}

template <typename CellT>
void DenseArrayStorage<CellT>::clear(sycl::queue& q) {}

template <typename CellT>
template <AccessMode ModeT, sycl::access::target TargetT>
template <sycl::access::target TestTargetT>
DenseArrayStorage<CellT>::Accessor<ModeT, TargetT>::Accessor(
    typename std::enable_if<TestTargetT == sycl::access::target::host_buffer,
                            sycl::buffer<typename CellT::Content, 3>&>::type
        buffer)
    : access_{buffer.template get_access<AccessModeInfo<ModeT>::sycl_value>()} {
}

template <typename CellT>
template <AccessMode ModeT, sycl::access::target TargetT>
DenseArrayStorage<CellT>::Accessor<ModeT, TargetT>::Accessor(
    sycl::buffer<typename CellT::Content, 3>& buffer, sycl::handler& cgh)
    : access_{buffer.template get_access<AccessModeInfo<ModeT>::sycl_value,
                                         TargetT>(cgh)} {}

template <typename CellT>
template <AccessMode ModeT, sycl::access::target TargetT>
typename DenseArrayStorage<CellT>::template Accessor<ModeT, TargetT>::AccessType
DenseArrayStorage<CellT>::Accessor<ModeT, TargetT>::operator[](
    const Position3s& p) const {
  return access_[sycl::id<3>{p.x, p.y, p.z}];
}

template <typename CellT>
template <AccessMode ModeT>
typename DenseArrayStorage<CellT>::template Accessor<
    ModeT, sycl::access::target::host_buffer>
DenseArrayStorage<CellT>::get_access(const Position3s& offset,
                                     const Extent3s& range) {
  return Accessor<ModeT, sycl::access::target::host_buffer>{buffer_};
}

template <typename CellT>
template <AccessMode ModeT>
typename DenseArrayStorage<CellT>::template Accessor<
    ModeT, sycl::access::target::global_buffer>
DenseArrayStorage<CellT>::get_access(sycl::handler& cgh,
                                     const Position3s& offset,
                                     const Extent3s& range) {
  return Accessor<ModeT, sycl::access::target::global_buffer>{buffer_, cgh};
}

}  // namespace sycl_planning
