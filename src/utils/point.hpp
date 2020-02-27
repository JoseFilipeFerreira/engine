#ifndef POINT_H
#define POINT_H
#include <array>
#include <string>
#include <tuple>

class Point {
private:
    float _x, _y, _z;

public:
    Point(float, float, float);
    std::string to_string() const;
};

#endif // POINT_H
