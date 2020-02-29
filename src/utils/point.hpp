#ifndef POINT_H
#define POINT_H
#include <array>
#include <string>
#include <tuple>

class Point_Spherical {
private:
    float _radius, _inclination, _azimuth;

public:
    Point_Spherical(float, float, float);
    std::string to_string() const;
    float radius() const { return _radius; }
    float inclination() const { return _inclination; }
    float azimuth() const { return _azimuth; }
};

class Point {
private:
    float _x, _y, _z;

public:
    Point(float, float, float);
    Point(Point_Spherical);
    std::string to_string() const;
    float x() const { return _x; }
    float y() const { return _y; }
    float z() const { return _z; }
};


#endif // POINT_H
