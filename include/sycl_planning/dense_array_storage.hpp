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
    explicit Accessor(
        typename std::enable_if<
            TestTargetT == sycl::access::target::host_buffer,
            sycl::buffer<typename CellT::Content, 3>&>::type buffer,
        const Position3s& offset, const Extent3s& extent);
    Accessor(sycl::buffer<typename CellT::Content, 3>& buffer,
             sycl::handler& cgh, const Position3s& offset,
             const Extent3s& extent);

    Position3s offset() const;
    Extent3s extent() const;

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
  Accessor<ModeT, sycl::access::target::host_buffer> get_host_access(
      const Position3s& offset = Position3s::origin,
      const Extent3s& extent = Extent3s::unbounded);
  template <AccessMode ModeT>
  Accessor<ModeT, sycl::access::target::global_buffer> get_access(
      sycl::handler& cgh, const Position3s& offset = Position3s::origin,
      const Extent3s& extent = Extent3s::unbounded);

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
void DenseArrayStorage<CellT>::clear(sycl::queue& q) {
  q.submit([&](sycl::handler& cgh) {
    auto access = get_access<sycl_planning::AccessMode::OVERWRITE>(cgh);

    cgh.parallel_for<class ClearData>(buffer_.get_range(), [=](sycl::id<3> id) {
      CellT::clear(access[Position3s{id.get(0), id.get(1), id.get(2)}]);
    });
  });
}

template <typename CellT>
template <AccessMode ModeT, sycl::access::target TargetT>
template <sycl::access::target TestTargetT>
DenseArrayStorage<CellT>::Accessor<ModeT, TargetT>::Accessor(
    typename std::enable_if<TestTargetT == sycl::access::target::host_buffer,
                            sycl::buffer<typename CellT::Content, 3>&>::type
        buffer,
    const Position3s& offset, const Extent3s& extent)
    : access_{buffer.template get_access<AccessModeInfo<ModeT>::sycl_value>(
          sycl::range<3>{extent.x, extent.y, extent.z},
          sycl::id<3>{offset.x, offset.y, offset.z})} {}

template <typename CellT>
template <AccessMode ModeT, sycl::access::target TargetT>
DenseArrayStorage<CellT>::Accessor<ModeT, TargetT>::Accessor(
    sycl::buffer<typename CellT::Content, 3>& buffer, sycl::handler& cgh,
    const Position3s& offset, const Extent3s& extent)
    : access_{
          buffer
              .template get_access<AccessModeInfo<ModeT>::sycl_value, TargetT>(
                  cgh, sycl::range<3>{extent.x, extent.y, extent.z},
                  sycl::id<3>{offset.x, offset.y, offset.z})} {}

template <typename CellT>
template <AccessMode ModeT, sycl::access::target TargetT>
Position3s DenseArrayStorage<CellT>::Accessor<ModeT, TargetT>::offset() const {
  const auto acc_offset = access_.get_offset();
  return Position3s{acc_offset.get(0), acc_offset.get(1), acc_offset.get(2)};
}

template <typename CellT>
template <AccessMode ModeT, sycl::access::target TargetT>
Extent3s DenseArrayStorage<CellT>::Accessor<ModeT, TargetT>::extent() const {
  const auto acc_range = access_.get_range();
  return Extent3s{acc_range.get(0), acc_range.get(1), acc_range.get(2)};
}

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
DenseArrayStorage<CellT>::get_host_access(const Position3s& offset,
                                          const Extent3s& extent) {
  const auto full_extent = this->extent();
  const auto clamp_extent =
      Extent3s{full_extent.x - offset.x, full_extent.y - offset.y,
               full_extent.z - offset.z};
  return Accessor<ModeT, sycl::access::target::host_buffer>{
      buffer_, offset, extent.clamp(clamp_extent)};
}

template <typename CellT>
template <AccessMode ModeT>
typename DenseArrayStorage<CellT>::template Accessor<
    ModeT, sycl::access::target::global_buffer>
DenseArrayStorage<CellT>::get_access(sycl::handler& cgh,
                                     const Position3s& offset,
                                     const Extent3s& extent) {
  const auto full_extent = this->extent();
  const auto clamp_extent =
      Extent3s{full_extent.x - offset.x, full_extent.y - offset.y,
               full_extent.z - offset.z};
  return Accessor<ModeT, sycl::access::target::global_buffer>{
      buffer_, cgh, offset, extent.clamp(clamp_extent)};
}

}  // namespace sycl_planning
