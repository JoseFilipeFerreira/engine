#include "utils/point.hpp"

#include <cmath>
#include <sstream>

float normalize_angle(float a) {
  if (a < -M_PI)
    return normalize_angle(a + 2 * M_PI);
  if (a > M_PI)
    return normalize_angle(a - 2 * M_PI);
  return a;
}

Vector::Vector(float x, float y, float z) : _x(x), _y(y), _z(z) {}

Vector::Vector(Point p1, Point p2) {
  _x = p2.x() - p1.x();
  _y = p2.y() - p1.y();
  _z = p2.z() - p1.z();
}

Vector::Vector(VectorSpherical p) {
  _x = p.radius() * sin(p.inclination()) * sin(p.azimuth());
  _y = p.radius() * cos(p.inclination());
  _z = p.radius() * sin(p.inclination()) * cos(p.azimuth());
}

auto Vector::unit() const -> Vector {
  float d = std::sqrt(_x * _x + _y * _y + _z * _z);
  return Vector(_x / d, _y / d, _z / d);
}

auto Vector::hadamard(Vector v) const -> Vector {
  return Vector(_x * v.x(), _y * v.y(), _z * v.z());
}

auto Vector::hadamard(float x, float y, float z) const -> Vector {
  return Vector(_x * x, _y * y, _z * z);
}

auto VectorSpherical::radius(float x) const -> VectorSpherical {
  return VectorSpherical(_radius + x, _inclination, _azimuth);
}

auto VectorSpherical::inclination(float x) const -> VectorSpherical {
  return VectorSpherical(_radius, normalize_angle(_inclination + x), _azimuth);
}
auto VectorSpherical::azimuth(float x) const -> VectorSpherical {
  return VectorSpherical(_radius, _inclination, normalize_angle(_azimuth + x));
}

VectorSpherical::VectorSpherical(float radius, float inclination, float azimuth)
    : _radius(radius), _inclination(inclination), _azimuth(azimuth) {}

VectorSpherical::VectorSpherical(Vector p) {
  _radius = sqrt(p.x() * p.x() + p.y() * p.y() + p.z() * p.z());

  if (_radius == 0)
    _inclination = 0;
  else
    _inclination = acos(p.y() / _radius);

  _azimuth = atan2(p.x(), p.z());
}

std::string VectorSpherical::to_string() const {
  std::ostringstream sstream;
  sstream << _radius << ' ' << _inclination << ' ' << _azimuth;
  return sstream.str();
}

Point::Point(float x, float y, float z) : _x(x), _y(y), _z(z) {}

auto Point::x(float v) const -> Point { return Point(_x + v, _y, _z); }
auto Point::y(float v) const -> Point { return Point(_x, _y + v, _z); }
auto Point::z(float v) const -> Point { return Point(_x, _y, _z + v); }

PointSpherical::PointSpherical(float radius, float inclination, float azimuth)
    : _radius(radius), _inclination(inclination), _azimuth(azimuth) {}

PointSpherical::PointSpherical(Point p) {
  _radius = sqrt(p.x() * p.x() + p.y() * p.y() + p.z() * p.z());

  if (_radius == 0)
    _inclination = 0;
  else
    _inclination = acos(p.y() / _radius);

  _azimuth = atan2(p.x(), p.z());
}

Point::Point(PointSpherical p) {
  _x = p.radius() * sin(p.inclination()) * sin(p.azimuth());
  _y = p.radius() * cos(p.inclination());
  _z = p.radius() * sin(p.inclination()) * cos(p.azimuth());
}

auto PointSpherical::radius(float x) const -> PointSpherical {
  return PointSpherical(_radius + x, _inclination, _azimuth);
}

auto PointSpherical::inclination(float x) const -> PointSpherical {
  return PointSpherical(_radius, normalize_angle(_inclination + x), _azimuth);
}
auto PointSpherical::azimuth(float x) const -> PointSpherical {
  return PointSpherical(_radius, _inclination, normalize_angle(_azimuth + x));
}

std::string PointSpherical::to_string() const {
  std::ostringstream sstream;
  sstream << _radius << ' ' << _inclination << ' ' << _azimuth;
  return sstream.str();
}

std::string Point::to_string() const {
  std::ostringstream sstream;
  sstream << _x << ' ' << _y << ' ' << _z;
  return sstream.str();
}
