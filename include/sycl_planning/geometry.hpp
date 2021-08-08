#pragma once

#include <cstddef>

namespace sycl_planning {

/*
 * Vector3
 */
template <typename T, template <typename> typename ConcreteT>
struct Vector3 {
  Vector3();
  Vector3(T x, T y, T z);

  template <typename TargetT>
  ConcreteT<TargetT> cast() const;
  template <template <typename> typename TargetConcreteT>
  TargetConcreteT<T> as() const;

  T x;
  T y;
  T z;
};

template <typename T, template <typename> typename ConcreteT>
bool operator==(const Vector3<T, ConcreteT>& v1,
                const Vector3<T, ConcreteT>& v2);
template <typename T, template <typename> typename ConcreteT>
bool operator!=(const Vector3<T, ConcreteT>& v1,
                const Vector3<T, ConcreteT>& v2);

/*
 * Position3
 */
template <typename T>
struct Position3 : public Vector3<T, Position3> {
  Position3();
  Position3(T x, T y, T z);

  static const Position3<T> origin;
};

using Position3d = Position3<double>;
using Position3f = Position3<float>;
using Position3i = Position3<int>;
using Position3s = Position3<std::size_t>;

/*
 * Translation3
 */
template <typename T>
struct Translation3 : public Vector3<T, Translation3> {
  Translation3();
  Translation3(T x, T y, T z);

  static const Translation3<T> unitX;
  static const Translation3<T> unitY;
  static const Translation3<T> unitZ;
};

using Translation3d = Translation3<double>;
using Translation3f = Translation3<float>;
using Translation3i = Translation3<int>;
using Translation3s = Translation3<std::size_t>;

template <typename T>
Translation3<T> operator-(const Translation3<T>& t);
template <typename T>
Translation3<T> operator+(const Translation3<T>& t1, const Translation3<T>& t2);
template <typename T>
Translation3<T>& operator+=(Translation3<T>& t1, const Translation3<T>& t2);
template <typename T>
Translation3<T> operator-(const Translation3<T>& t1, const Translation3<T>& t2);
template <typename T>
Translation3<T>& operator-=(Translation3<T>& t1, const Translation3<T>& t2);

template <typename T>
Translation3<T> operator*(const Translation3<T>& t, T s);
template <typename T>
Translation3<T> operator*(T s, const Translation3<T>& t);
template <typename T>
Translation3<T>& operator*=(Translation3<T>& t, T s);
template <typename T>
Translation3<T> operator/(const Translation3<T>& t, T s);
template <typename T>
Translation3<T> operator/=(Translation3<T>& t, T s);

template <typename T>
Translation3<T> operator-(const Position3<T>& p1, const Position3<T>& p2);
template <typename T>
Position3<T> operator+(const Position3<T>& p, const Translation3<T>& t);
template <typename T>
Position3<T>& operator+=(Position3<T>& p, const Translation3<T>& t);
template <typename T>
Position3<T> operator-(const Position3<T>& p, const Translation3<T>& t);
template <typename T>
Position3<T>& operator-=(Position3<T>& p, const Translation3<T>& t);

/*
 * Template definitions
 */

template <typename T, template <typename> typename ConcreteT>
Vector3<T, ConcreteT>::Vector3() : x{0}, y{0}, z{0} {}

template <typename T, template <typename> typename ConcreteT>
Vector3<T, ConcreteT>::Vector3(T x, T y, T z) : x{x}, y{y}, z{z} {}

template <typename T, template <typename> typename ConcreteT>
template <typename TargetT>
ConcreteT<TargetT> Vector3<T, ConcreteT>::cast() const {
  return ConcreteT<TargetT>{x, y, z};
}

template <typename T, template <typename> typename ConcreteT>
template <template <typename> typename TargetConcreteT>
TargetConcreteT<T> Vector3<T, ConcreteT>::as() const {
  return TargetConcreteT<T>{x, y, z};
}

template <typename T, template <typename> typename ConcreteT>
bool operator==(const Vector3<T, ConcreteT>& v1,
                const Vector3<T, ConcreteT>& v2) {
  return (v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z);
}

template <typename T, template <typename> typename ConcreteT>
bool operator!=(const Vector3<T, ConcreteT>& v1,
                const Vector3<T, ConcreteT>& v2) {
  return !(v1 == v2);
}

template <typename T>
Position3<T>::Position3() {}

template <typename T>
Position3<T>::Position3(T x, T y, T z) : Vector3<T, Position3>{x, y, z} {}

template <typename T>
const Position3<T> Position3<T>::origin = Position3<T>{0, 0, 0};

template <typename T>
Translation3<T>::Translation3() {}

template <typename T>
Translation3<T>::Translation3(T x, T y, T z)
    : Vector3<T, Translation3>{x, y, z} {}

template <typename T>
Translation3<T> operator-(const Translation3<T>& t) {
  return Translation3<T>{-t.x, -t.y, -t.z};
}

template <typename T>
const Translation3<T> Translation3<T>::unitX = Translation3<T>{1, 0, 0};

template <typename T>
const Translation3<T> Translation3<T>::unitY = Translation3<T>{0, 1, 0};

template <typename T>
const Translation3<T> Translation3<T>::unitZ = Translation3<T>{0, 0, 1};

template <typename T>
Translation3<T> operator+(const Translation3<T>& t1,
                          const Translation3<T>& t2) {
  return Translation3<T>{t1.x + t2.x, t1.y + t2.y, t1.z + t2.z};
}

template <typename T>
Translation3<T>& operator+=(Translation3<T>& t1, const Translation3<T>& t2) {
  t1 = t1 + t2;
  return t1;
}

template <typename T>
Translation3<T> operator-(const Translation3<T>& t1,
                          const Translation3<T>& t2) {
  return t1 + (-t2);
}

template <typename T>
Translation3<T>& operator-=(Translation3<T>& t1, const Translation3<T>& t2) {
  t1 = t1 - t2;
  return t1;
}

template <typename T>
Translation3<T> operator*(const Translation3<T>& t, T s) {
  return Translation3<T>{t.x * s, t.y * s, t.z * s};
}

template <typename T>
Translation3<T> operator*(T s, const Translation3<T>& t) {
  return t * s;
}

template <typename T>
Translation3<T>& operator*=(Translation3<T>& t, T s) {
  t = t * s;
  return t;
}

template <typename T>
Translation3<T> operator/(const Translation3<T>& t, T s) {
  return Translation3<T>{t.x / s, t.y / s, t.z / s};
}

template <typename T>
Translation3<T> operator/=(Translation3<T>& t, T s) {
  t = t / s;
  return t;
}

template <typename T>
Translation3<T> operator-(const Position3<T>& p1, const Position3<T>& p2) {
  return p1.template as<Translation3>() - p2.template as<Translation3>();
}

template <typename T>
Position3<T> operator+(const Position3<T>& p, const Translation3<T>& t) {
  return (p.template as<Translation3>() + t).template as<Position3>();
}

template <typename T>
Position3<T> operator-(const Position3<T>& p, const Translation3<T>& t) {
  return p + (-t);
}

template <typename T>
Position3<T>& operator+=(Position3<T>& p, const Translation3<T>& t) {
  p = p + t;
  return p;
}

template <typename T>
Position3<T>& operator-=(Position3<T>& p, const Translation3<T>& t) {
  p = p - t;
  return p;
}

}  // namespace sycl_planning
