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

    ModelPoint(PointSpherical const& ps, VectorSpherical const& vs, float x, float y)
        : _points(Point(ps)), _vector(Vector(vs)), _x(x), _y(y){};

    ModelPoint(Point const& ps, VectorSpherical const& vs, float x, float y)
        : _points(ps), _vector(Vector(vs)), _x(x), _y(y){};

    ModelPoint(PointSpherical const& ps, Vector const& vs, float x, float y)
        : _points(Point(ps)), _vector(vs), _x(x), _y(y){};

    auto friend operator<<(std::ostream& oss, ModelPoint const& mp) -> std::ostream& {
        oss << mp._points << ' ';
        oss << mp._vector << ' ';
        oss << mp._x << ' ';
        oss << mp._y;
        return oss;
    }
};

#endif // MODELPOINT_H
