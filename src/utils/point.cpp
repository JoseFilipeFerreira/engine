#include "utils/point.hpp"

#include <cmath>
#include <sstream>

float normalize_angle(float a) {
    if (a < -M_PI) return normalize_angle(a + 2.0f * M_PI);
    if (a > M_PI) return normalize_angle(a - 2.0f * M_PI);
    return a;
}

Vector::Vector(float x, float y, float z): _x(x), _y(y), _z(z) {}

Vector::Vector(Point const& p1, Point const& p2)
    : _x(p2.x() - p1.x()), _y(p2.y() - p1.y()), _z(p2.z() - p1.z()) {}

Vector::Vector(Point const& p): _x(p.x()), _y(p.y()), _z(p.z()) {}

Vector::Vector(VectorSpherical const& p)
    : _x(p.radius() * sin(p.inclination()) * sin(p.azimuth())),
      _y(p.radius() * cos(p.inclination())),
      _z(p.radius() * sin(p.inclination()) * cos(p.azimuth())) {}

auto Vector::normalize() const -> Vector {
    float d = std::sqrt(_x * _x + _y * _y + _z * _z);
    return Vector(_x / d, _y / d, _z / d);
}

auto Vector::cross(Vector v) const -> Vector {
    return Vector(
        _y * v.z() - _z * v.y(),
        _z * v.x() - _x * v.z(),
        _x * v.y() - _y * v.z());
}

auto VectorSpherical::add_radius(float x) const -> VectorSpherical {
    return VectorSpherical(_radius + x, _inclination, _azimuth);
}

auto VectorSpherical::normalize() const -> VectorSpherical {
    return VectorSpherical(1, _inclination, _azimuth);
}

auto VectorSpherical::add_inclination(float x) const -> VectorSpherical {
    return VectorSpherical(
        _radius, normalize_angle(_inclination + x), _azimuth);
}
auto VectorSpherical::add_azimuth(float x) const -> VectorSpherical {
    return VectorSpherical(
        _radius, _inclination, normalize_angle(_azimuth + x));
}

VectorSpherical::VectorSpherical(float radius, float inclination, float azimuth)
    : _radius(radius), _inclination(inclination), _azimuth(azimuth) {}

VectorSpherical::VectorSpherical(Vector const& p)
    : _radius(sqrt(p.x() * p.x() + p.y() * p.y() + p.z() * p.z())),
      _inclination(_radius == 0 ? 0 : std::acos(p.y() / _radius)),
      _azimuth(std::atan2(p.x(), p.z())) {}

VectorSpherical::VectorSpherical(Point const& p0, Point const& p1) {
    auto v = VectorSpherical(Vector(p0, p1));
    _radius = v.radius();
    _inclination = v.inclination();
    _azimuth = v.azimuth();
}

Point::Point(float x, float y, float z): _x(x), _y(y), _z(z) {}

auto Point::add_x(float v) const -> Point {
    return Point(_x + v, _y, _z);
}
auto Point::add_y(float v) const -> Point {
    return Point(_x, _y + v, _z);
}
auto Point::add_z(float v) const -> Point {
    return Point(_x, _y, _z + v);
}

PointSpherical::PointSpherical(float radius, float inclination, float azimuth)
    : _radius(radius), _inclination(inclination), _azimuth(azimuth) {}

PointSpherical::PointSpherical(Point const& p)
    : _radius(sqrt(p.x() * p.x() + p.y() * p.y() + p.z() * p.z())),
      _inclination(_radius == 0 ? 0 : std::acos(p.y() / _radius)),
      _azimuth(std::atan2(p.x(), p.z())) {}

Point::Point(PointSpherical const& p)
    : _x(p.radius() * sin(p.inclination()) * sin(p.azimuth())),
      _y(p.radius() * cos(p.inclination())),
      _z(p.radius() * sin(p.inclination()) * cos(p.azimuth())) {}

auto PointSpherical::add_radius(float x) const -> PointSpherical {
    return PointSpherical(_radius + x, _inclination, _azimuth);
}

auto PointSpherical::add_inclination(float x) const -> PointSpherical {
    return PointSpherical(_radius, normalize_angle(_inclination + x), _azimuth);
}
auto PointSpherical::add_azimuth(float x) const -> PointSpherical {
    return PointSpherical(_radius, _inclination, normalize_angle(_azimuth + x));
}

auto Point::normalized_vector() const -> Vector {
    return Vector(_x, _y, _z).normalize();
}

auto PointSpherical::normalized_vector() const -> VectorSpherical {
    return VectorSpherical(1, _inclination, _azimuth);
}

auto operator<<(std::ostream& oss, VectorSpherical const& vs) -> std::ostream& {
    oss << vs._radius << ' ' << vs._inclination << ' ' << vs._azimuth;
    return oss;
}

auto operator<<(std::ostream& oss, PointSpherical const& ps) -> std::ostream& {
    oss << ps._radius << ' ' << ps._inclination << ' ' << ps._azimuth;
    return oss;
}

auto operator<<(std::ostream& oss, Point const& p) -> std::ostream& {
    oss << p._x << ' ' << p._y << ' ' << p._z;
    return oss;
}

auto operator<<(std::ostream& oss, Vector const& v) -> std::ostream& {
    oss << v._x << ' ' << v._y << ' ' << v._z;
    return oss;
}
