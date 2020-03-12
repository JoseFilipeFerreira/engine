#ifndef POINT_H
#define POINT_H
#include <array>
#include <string>
#include <tuple>

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
    Vector unit() const;
    Vector hadamard(Vector) const;
    Vector hadamard(float, float, float) const;
    Vector operator+(Vector v) const {
        return Vector(_x + v.x(), _y + v.y(), _z + v.z());
    }
    Vector operator*(int s) const { return Vector(_x * s, _y * s, _z * s); }
    Vector operator/(int s) const { return Vector(_x / s, _y / s, _z / s); }
};

class VectorSpherical {
  private:
    float _radius, _inclination, _azimuth;

  public:
    VectorSpherical(float, float, float);
    VectorSpherical(Vector);
    std::string to_string() const;
    auto constexpr radius() const noexcept -> float { return _radius; }
    auto constexpr inclination() const noexcept -> float {
        return _inclination;
    }
    auto constexpr azimuth() const noexcept -> float { return _azimuth; }
    VectorSpherical operator+(VectorSpherical const& v) {
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
    void x(float x) { _x += x; }
    void y(float x) { _y += x; }
    void z(float x) { _z += x; }
    Point operator+(Vector const& p) {
        return Point(_x + p.x(), _y + p.y(), _z + p.z());
    }
    Point operator+(VectorSpherical const& vs) {
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
    auto constexpr inclination() const noexcept -> float {
        return _inclination;
    }
    auto constexpr azimuth() const noexcept -> float { return _azimuth; }
    void radius(float x) { _radius += x; }
    void inclination(float x) { _inclination += x; }
    void azimuth(float x) { _azimuth += x; }
    PointSpherical operator+(Vector const& v) {
        return PointSpherical(Point(*this) + v);
    }
    PointSpherical operator+(VectorSpherical const& v) {
        return PointSpherical(Point(*this) + Vector(v));
    }
};

#endif // POINT_H
