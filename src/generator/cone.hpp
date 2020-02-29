#ifndef PLANE_H
#define PLANE_H
#include "primitives.hpp"

class Cone : public Primitive
{
private:
    double _radius;
    double _slices;
    double _stacks;

public:
    Cone(int argc, char** argv);
    std::vector<Point> draw() const;
};

#endif // PLANE_H
