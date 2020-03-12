#include "utils/point.hpp"

#include <cmath>
#include <iostream>
#include <math.h>
#include <sstream>

Vector::Vector(float x, float y, float z): _x(x), _y(y), _z(z) {}

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

Vector Vector::unit() const {
    float d = std::sqrt(_x * _x + _y * _y + _z * _z);
    return Vector(_x / d, _y / d, _z / d);
}

Vector Vector::hadamard(Vector v) const {
    return Vector(_x * v.x(), _y * v.y(), _z * v.z());
}

Vector Vector::hadamard(float x, float y, float z) const {
    return Vector(_x * x, _y * y, _z * z);
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

Point::Point(float x, float y, float z): _x(x), _y(y), _z(z) {}

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
