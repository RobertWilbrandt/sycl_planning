#pragma once

#include <cmath>
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

  T norm() const;
  T normSquared() const;
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
 * Extent
 */
template <typename T>
struct Extent : public Vector3<T, Extent> {
  Extent();
  Extent(T x, T y, T z);
};

using Extentd = Extent<double>;
using Extentf = Extent<float>;
using Extenti = Extent<int>;
using Extents = Extent<std::size_t>;

/*
 * Quaternion
 */

template <typename T>
struct Quaternion {
  Quaternion();
  Quaternion(T a, T b, T c, T d);

  static const Quaternion<T> identity;

  template <typename TargetT>
  Quaternion<TargetT> cast() const;

  Quaternion<T> conjugate() const;

  T a;
  T b;
  T c;
  T d;
};

using Quaterniond = Quaternion<double>;
using Quaternionf = Quaternion<float>;

template <typename T>
bool operator==(const Quaternion<T>& q1, const Quaternion<T>& q2);
template <typename T>
bool operator!=(const Quaternion<T>& q1, const Quaternion<T>& q2);

template <typename T>
Quaternion<T> operator*(const Quaternion<T>& q1, const Quaternion<T>& q2);
template <typename T>
Quaternion<T>& operator*=(Quaternion<T>& q1, const Quaternion<T>& q2);

/*
 * Orientation
 */
template <typename T>
class Orientation3 {
 public:
  Orientation3();
  explicit Orientation3(const Quaternion<T>& q);

  template <template <typename> typename TargetT>
  TargetT<T> as() const;

 private:
  Quaternion<T> q_;
};

using Orientation3d = Orientation3<double>;
using Orientation3f = Orientation3<float>;

/*
 * Rotation
 */
template <typename T>
class Rotation3 {
 public:
  Rotation3();
  explicit Rotation3(const Quaternion<T>& q);

  template <template <typename> typename TargetT>
  TargetT<T> as() const;

  static Rotation3<T> aroundX(T angle);
  static Rotation3<T> aroundY(T angle);
  static Rotation3<T> aroundZ(T angle);

  Translation3<T> operator*(const Translation3<T>& t) const;

 private:
  Quaternion<T> q_;
};

using Rotation3d = Rotation3<double>;
using Rotation3f = Rotation3<float>;

template <typename T>
Rotation3<T> operator*(const Rotation3<T>& r1, const Rotation3<T>& r2);
template <typename T>
Rotation3<T>& operator*=(Rotation3<T>& r1, const Rotation3<T>& r2);

template <typename T>
Orientation3<T> operator*(const Rotation3<T>& r, const Orientation3<T>& o);

/*
 * Pose
 */
template <typename T>
struct Pose3 {
 public:
  Pose3();
  Pose3(const Position3<T>& position, const Orientation3<T>& orientation);

  Position3<T> position;
  Orientation3<T> orientation;
};

using Pose3d = Pose3<double>;
using Pose3f = Pose3<float>;

/*
 * Transform
 */
template <typename T>
struct Transform3 {
 public:
  Transform3();
  Transform3(const Translation3<T>& translation, const Rotation3<T>& rotation);

  Translation3<T> translation;
  Rotation3<T> rotation;
};

using Transform3d = Transform3<double>;
using Transform3f = Transform3<float>;

template <typename T>
Transform3<T> operator*(const Transform3<T>& t1, const Transform3<T>& t2);
template <typename T>
Transform3<T>& operator*=(Transform3<T>& t1, const Transform3<T>& t2);

template <typename T>
Pose3<T> operator*(const Transform3<T>& t, const Pose3<T>& p);

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
T Translation3<T>::norm() const {
  return std::sqrt(normSquared());
}

template <typename T>
T Translation3<T>::normSquared() const {
  return Vector3<T, Translation3>::x * Vector3<T, Translation3>::x +
         Vector3<T, Translation3>::y * Vector3<T, Translation3>::y +
         Vector3<T, Translation3>::z * Vector3<T, Translation3>::z;
}

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

template <typename T>
Extent<T>::Extent() {}

template <typename T>
Extent<T>::Extent(T x, T y, T z) : Vector3<T, Extent>{x, y, z} {}

template <typename T>
Quaternion<T>::Quaternion() : a{0}, b{0}, c{0}, d{0} {}

template <typename T>
Quaternion<T>::Quaternion(T a, T b, T c, T d) : a{a}, b{b}, c{c}, d{d} {}

template <typename T>
const Quaternion<T> Quaternion<T>::identity = Quaternion<T>{1, 0, 0, 0};

template <typename T>
template <typename TargetT>
Quaternion<TargetT> Quaternion<T>::cast() const {
  return Quaternion<TargetT>{TargetT{a}, TargetT{b}, TargetT{c}, TargetT{d}};
}

template <typename T>
Quaternion<T> Quaternion<T>::conjugate() const {
  return Quaternion<T>{a, -b, -c, -d};
}

template <typename T>
bool operator==(const Quaternion<T>& q1, const Quaternion<T>& q2) {
  return (q1.a == q2.a) && (q1.b == q2.b) && (q1.c == q2.c) && (q1.d == q2.d);
}

template <typename T>
Quaternion<T> operator*(const Quaternion<T>& q1, const Quaternion<T>& q2) {
  return Quaternion<T>{q1.a * q2.a - q1.b * q2.b - q1.c * q2.c - q1.d * q2.d,
                       q1.a * q2.b + q1.b * q2.a + q1.c * q2.d - q1.d * q2.c,
                       q1.a * q2.c - q1.b * q2.d + q1.c * q2.a + q1.d * q2.b,
                       q1.a * q2.d + q1.b * q2.c - q1.c * q2.b + q1.d * q2.a};
}

template <typename T>
Orientation3<T>::Orientation3() {}

template <typename T>
Orientation3<T>::Orientation3(const Quaternion<T>& q) : q_{q} {}

template <typename T>
template <template <typename> typename TargetT>
TargetT<T> Orientation3<T>::as() const {
  return TargetT<T>{q_};
}

template <typename T>
Rotation3<T>::Rotation3() {}

template <typename T>
Rotation3<T>::Rotation3(const Quaternion<T>& q) : q_{q} {}

template <typename T>
template <template <typename> typename TargetT>
TargetT<T> Rotation3<T>::as() const {
  return TargetT<T>{q_};
}

template <typename T>
Rotation3<T> Rotation3<T>::aroundX(T angle) {
  return Rotation3{
      Quaternion<T>{std::cos(angle / 2), std::sin(angle / 2), 0, 0}};
}

template <typename T>
Rotation3<T> Rotation3<T>::aroundY(T angle) {
  return Rotation3{
      Quaternion<T>{std::cos(angle / 2), 0, std::sin(angle / 2), 0}};
}

template <typename T>
Rotation3<T> Rotation3<T>::aroundZ(T angle) {
  return Rotation3{
      Quaternion<T>{std::cos(angle / 2), 0, 0, std::sin(angle / 2)}};
}

template <typename T>
Translation3<T> Rotation3<T>::operator*(const Translation3<T>& t) const {
  Quaternion<T> result_quat =
      q_ * Quaternion<T>{0, t.x, t.y, t.z} * q_.conjugate();
  return Translation3<T>{result_quat.b, result_quat.c, result_quat.d};
}

template <typename T>
Rotation3<T> operator*(const Rotation3<T>& r1, const Rotation3<T>& r2) {
  return Rotation3<T>{r1.template as<Quaternion>() *
                      r2.template as<Quaternion>()};
}

template <typename T>
Rotation3<T>& operator*=(Rotation3<T>& r1, const Rotation3<T>& r2) {
  r1 = r1 * r2;
  return r1;
}

template <typename T>
Orientation3<T> operator*(const Rotation3<T>& r, const Orientation3<T>& o) {
  return (r * o.template as<Rotation3>()).template as<Orientation3>();
}

template <typename T>
Pose3<T>::Pose3() {}

template <typename T>
Pose3<T>::Pose3(const Position3<T>& position,
                const Orientation3<T>& orientation)
    : position{position}, orientation{orientation} {}

template <typename T>
Transform3<T>::Transform3() {}

template <typename T>
Transform3<T>::Transform3(const Translation3<T>& translation,
                          const Rotation3<T>& rotation)
    : translation{translation}, rotation{rotation} {}

template <typename T>
Transform3<T> operator*(const Transform3<T>& t1, const Transform3<T>& t2) {
  return Transform3<T>{t1 + t1.rotation * t2.translation,
                       t1.rotation * t2.rotation};
}

template <typename T>
Transform3<T>& operator*=(Transform3<T>& t1, const Transform3<T>& t2) {
  t1 = t1 * t2;
  return t1;
}

template <typename T>
Pose3<T> operator*(const Transform3<T>& t, const Pose3<T>& p) {
  return Pose3<T>{Position3<T>{} + t.translation +
                      t.rotation * p.position.template as<Translation3>(),
                  t.rotation * p.orientation};
}

}  // namespace sycl_planning
