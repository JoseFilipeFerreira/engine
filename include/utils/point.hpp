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
    Vector(): _x(0), _y(0), _z(0){};
    Vector(float, float, float);
    Vector(Point const&, Point const&);
    Vector(Point const&);
    Vector(VectorSpherical const&);
    auto friend operator<<(std::ostream&, Vector const&) -> std::ostream&;
    auto constexpr x() const noexcept -> float { return _x; }
    auto constexpr y() const noexcept -> float { return _y; }
    auto constexpr z() const noexcept -> float { return _z; }
    auto mirror_x() const -> Vector { return Vector(-1 * _x, _y, _z); }
    auto mirror_y() const -> Vector { return Vector(_x, -1 * _y, _z); }
    auto mirror_z() const -> Vector { return Vector(_x, _y, -1 * _z); }
    auto normalize() const -> Vector;
    auto cross(Vector) const -> Vector;
    auto operator+(Vector const& v) const -> Vector {
        return Vector(_x + v.x(), _y + v.y(), _z + v.z());
    }
    auto operator*(float s) const -> Vector {
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
    VectorSpherical(Vector const&);
    VectorSpherical(Point const&, Point const&);
    auto friend operator<<(std::ostream&, VectorSpherical const&)
        -> std::ostream&;
    auto constexpr radius() const noexcept -> float { return _radius; }
    auto constexpr inclination() const noexcept -> float {
        return _inclination;
    }
    auto constexpr azimuth() const noexcept -> float { return _azimuth; }
    auto add_radius(float) const -> VectorSpherical;
    auto normalize() const -> VectorSpherical;
    auto add_inclination(float) const -> VectorSpherical;
    auto add_azimuth(float) const -> VectorSpherical;
    auto operator+(VectorSpherical const& v) const -> VectorSpherical {
        return VectorSpherical(Vector(*this) + Vector(v));
    }
    auto operator*(float s) const -> VectorSpherical {
        return VectorSpherical(_radius * s, _inclination, _azimuth);
    }
};

class Point {
  private:
    float _x, _y, _z;

  public:
    Point(): _x(0), _y(0), _z(0){};
    Point(float, float, float);
    Point(PointSpherical const&);
    auto friend operator<<(std::ostream&, Point const&) -> std::ostream&;
    auto constexpr x() const noexcept -> float { return _x; }
    auto constexpr y() const noexcept -> float { return _y; }
    auto constexpr z() const noexcept -> float { return _z; }
    auto mirror_x() const -> Point { return Point(-1 * _x, _y, _z); }
    auto mirror_y() const -> Point { return Point(_x, -1 * _y, _z); }
    auto mirror_z() const -> Point { return Point(_x, _y, -1 * _z); }
    auto add_x(float) const -> Point;
    auto add_y(float) const -> Point;
    auto add_z(float) const -> Point;
    auto normalized_vector() const -> Vector;
    auto operator+(Vector const& p) const -> Point {
        return Point(_x + p.x(), _y + p.y(), _z + p.z());
    }
    auto operator+(VectorSpherical const& vs) const -> Point {
        Vector v = Vector(vs);
        return Point(_x + v.x(), _y + v.y(), _z + v.z());
    }
};

class PointSpherical {
  private:
    float _radius, _inclination, _azimuth;

  public:
    PointSpherical(float, float, float);
    PointSpherical(Point const&);
    auto friend operator<<(std::ostream&, PointSpherical const&)
        -> std::ostream&;
    auto constexpr radius() const noexcept -> float { return _radius; }
    auto constexpr inclination() const noexcept -> float {
        return _inclination;
    }
    auto constexpr azimuth() const noexcept -> float { return _azimuth; }
    auto add_radius(float) const -> PointSpherical;
    auto add_inclination(float) const -> PointSpherical;
    auto add_azimuth(float) const -> PointSpherical;
    auto normalized_vector() const -> VectorSpherical;
    auto operator+(Vector const& v) const -> PointSpherical {
        return PointSpherical(Point(*this) + v);
    }
    auto operator+(VectorSpherical const& v) const -> PointSpherical {
        return PointSpherical(Point(*this) + Vector(v));
    }
};

#endif // POINT_H
