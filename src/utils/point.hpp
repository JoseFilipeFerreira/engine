#ifndef POINT_H
#define POINT_H
#include <array>
#include <string>
#include <tuple>

class PointSpherical {
private:
    float _radius, _inclination, _azimuth;

public:
    PointSpherical(float, float, float);
    std::string to_string() const;
    auto constexpr radius() const noexcept -> float { return _radius; }
    auto constexpr inclination() const noexcept -> float { return _inclination; }
    auto constexpr azimuth() const noexcept -> float { return _azimuth; }
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
    Point operator + (Point const &obj) { 
         return Point(_x + obj.x(), _y + obj.y(), _z + obj.z()); 
    }
};


#endif // POINT_H
