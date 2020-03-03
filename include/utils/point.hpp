#ifndef POINT_H
#define POINT_H
#include <array>
#include <string>
#include <tuple>

class Point;
class PointSpherical;
class Vector;

class Vector {
private:
    float _x, _y, _z;

public:
    Vector(float, float, float);
    Vector(Point, Point);
    std::string to_string() const;
    auto constexpr x() const noexcept -> float { return _x; }
    auto constexpr y() const noexcept -> float { return _y; }
    auto constexpr z() const noexcept -> float { return _z; }
    Vector unit();
    Vector hadamard(Vector);
    Vector hadamard(float, float, float);
    Vector operator * (int s) { 
         return Vector(_x * s, _y * s, _z *s); 
    }
    Vector operator / (int s) { 
         return Vector(_x / s, _y / s, _z / s); 
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
    Point operator + (Vector const &p) { 
         return Point(_x + p.x(), _y + p.y(), _z + p.z()); 
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
    PointSpherical operator + (Vector const &v) {
        return PointSpherical(Point(*this) + v); 
    }
};

#endif // POINT_H
