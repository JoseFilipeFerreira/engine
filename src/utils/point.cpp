#include "utils/point.hpp"
#include <sstream>
#include <cmath>

Vector::Vector(float x, float y, float z)
    : _x(x)
    , _y(y)
    , _z(z)
{
}

Vector Vector::unit() {
    float d = std::sqrt(_x*_x + _y*_y + _z*_z);
    return Vector(_x/d, _y/d, _z/d);
}

Vector Vector::hadamard(Vector v) {
    return Vector(_x * v.x(), _y * v.y(), _z * v.z());
}

Vector Vector::hadamard(float x, float y, float z) {
    return Vector(_x * x, _y * y, _z * z);
}

Point::Point(float x, float y, float z)
    : _x(x)
    , _y(y)
    , _z(z)
{
}

PointSpherical::PointSpherical(float radius, float inclination, float azimuth)
    : _radius(radius)
    , _inclination(inclination)
    , _azimuth(azimuth)
{
}

Point::Point(PointSpherical p) {
    _z = p.radius() * sin(p.inclination()) * cos(p.azimuth());
    _x = p.radius() * sin(p.inclination()) * sin(p.azimuth());
    _y = p.radius() * cos(p.inclination()); 
}

std::string PointSpherical::to_string() const {
    std::ostringstream sstream;
    sstream
        << _radius << ' '
        << _inclination << ' '
        << _azimuth;
    return sstream.str();
}

std::string Point::to_string() const {
    std::ostringstream sstream;
    sstream
        << _x << ' '
        << _y << ' '
        << _z;
    return sstream.str();
}
