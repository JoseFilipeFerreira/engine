#ifndef POINT_H
#define POINT_H
#include <cmath>
#include <string>

class Point;
class PointSpherical;
class Vector;
class VectorSpherical;

class Vector {
private:
  float _x, _y, _z;

public:
  Vector(float, float, float);
  Vector(Point, Point);
  Vector(VectorSpherical);
  std::string to_string() const;
  auto constexpr x() const noexcept -> float { return _x; }
  auto constexpr y() const noexcept -> float { return _y; }
  auto constexpr z() const noexcept -> float { return _z; }
  auto unit() const -> Vector;
  auto hadamard(Vector) const -> Vector;
  auto hadamard(float, float, float) const -> Vector;
  auto operator+(Vector v) const -> Vector {
    return Vector(_x + v.x(), _y + v.y(), _z + v.z());
  }
  auto operator*(int s) const -> Vector {
    return Vector(_x * s, _y * s, _z * s);
  }
  auto operator/(int s) const -> Vector {
    return Vector(_x / s, _y / s, _z / s);
  }
};

class VectorSpherical {
private:
  float _radius, _inclination, _azimuth;

public:
  VectorSpherical(float, float, float);
  VectorSpherical(Vector);
  std::string to_string() const;
  auto constexpr radius() const noexcept -> float { return _radius; }
  auto constexpr inclination() const noexcept -> float { return _inclination; }
  auto constexpr azimuth() const noexcept -> float { return _azimuth; }
  auto radius(float) const -> VectorSpherical;
  auto inclination(float) const -> VectorSpherical;
  auto azimuth(float) const -> VectorSpherical;
  auto operator+(VectorSpherical const &v) const -> VectorSpherical {
    return VectorSpherical(Vector(*this) + Vector(v));
  }
};

class Point {
private:
  float _x, _y, _z;

public:
  Point(float, float, float);
  Point(PointSpherical);
  std::string to_string() const;
  auto constexpr x() const noexcept -> float { return _x; }
  auto constexpr y() const noexcept -> float { return _y; }
  auto constexpr z() const noexcept -> float { return _z; }
  auto x(float) const -> Point;
  auto y(float) const -> Point;
  auto z(float) const -> Point;
  auto operator+(Vector const &p) const -> Point {
    return Point(_x + p.x(), _y + p.y(), _z + p.z());
  }
  auto operator+(VectorSpherical const &vs) const -> Point {
    Vector v = Vector(vs);
    return Point(_x + v.x(), _y + v.y(), _z + v.z());
  }
};

class PointSpherical {
private:
  float _radius, _inclination, _azimuth;

public:
  PointSpherical(float, float, float);
  PointSpherical(Point);
  std::string to_string() const;
  auto constexpr radius() const noexcept -> float { return _radius; }
  auto constexpr inclination() const noexcept -> float { return _inclination; }
  auto constexpr azimuth() const noexcept -> float { return _azimuth; }
  auto radius(float) const -> PointSpherical;
  auto inclination(float) const -> PointSpherical;
  auto azimuth(float) const -> PointSpherical;
  auto operator+(Vector const &v) const -> PointSpherical {
    return PointSpherical(Point(*this) + v);
  }
  auto operator+(VectorSpherical const &v) const -> PointSpherical {
    return PointSpherical(Point(*this) + Vector(v));
  }
};

#endif // POINT_H
