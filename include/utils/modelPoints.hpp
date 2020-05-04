#ifndef MODELPOINT_H
#define MODELPOINT_H
#include "utils/point.hpp"

#include <sstream>
#include <string>

class ModelPoint {
  private:
    Point _points;
    Vector _vector;
    float _x, _y;

  public:
    ModelPoint(Point const& p, Vector const& v, float x, float y)
        : _points(p), _vector(v), _x(x), _y(y){};

    auto friend operator<<(std::ostream& oss, ModelPoint const& mp) -> std::ostream& {
        oss << mp._points << ' ';
        oss << mp._vector << ' ';
        oss << mp._x << ' ';
        oss << mp._y;
        return oss;
    }
};

#endif // MODELPOINT_H
