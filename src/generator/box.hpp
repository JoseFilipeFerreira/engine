#ifndef PLANE_H
#define PLANE_H
#include "primitives.hpp"

class Box : public Primitive {
private:
    double _x;
    double _y;
    double _z;
    double _slices;

public:
    Box(int argc, char** argv);
    std::vector<Point> draw() const;
};

#endif // PLANE_H
