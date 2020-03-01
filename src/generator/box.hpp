#ifndef BOX_H
#define BOX_H
#include "primitives.hpp"

class Box : public Primitive {
private:
    float _x;
    float _y;
    float _z;
    int _slices;

public:
    Box(int argc, char** argv);
    std::vector<Point> draw() const;
};

#endif // PLANE_H
